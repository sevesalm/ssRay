#include "KDTree.h"
#include "KDNode.h"
#include "KDStackElement.h"
#include "KDSplitElement.h"
#include "Object.h"
#include <algorithm>

#define MAXOBJECTS 1
#define MAXDEPTH 34

KDTree::KDTree(std::vector<AABBox*>& objects, std::vector<Object*>& nonBoundableObjects, AABBox& bounds) : nonBoundableObjects(nonBoundableObjects)
{
    root = new KDNode();
    //std::cout<<" (node size: "<<sizeof(*root)<<"B) ";
    populate(root, bounds.P1, bounds.P2, objects, MAXDEPTH);
}

KDTree::~KDTree()
{
    // Delete all nodes below root node
    destroy(root);
    // Also delete the root node itself
    delete root;
}

/* Populates the node according to rules. */

/*
 * 1. Have a struct with one BBox point[axis], its type (P1 or P2) and assosiated object
 * 2. Put all in a vector
 * 3. Sort them.
 * 4. Calculate:
 *    CI*(Area(left)*NObjects(left)+Area(Right)*NObjects(right))/Area(total) + 3*CT
 * 5. Pick lowest point.
 * 6. Repear for 3 axes
 * 7. Compare to the case without split at all:
 *    CI*NObjects + CT
 * 8. Make decision and repeat for possible child nodes.
 * 
 */

#define CI 80 // Cost of one intersection
#define CT 2 // Cost of having to traverse 2 child nodes
#define EB 1 // Empty node bonus

#define NEW
#ifdef NEW
void KDTree::populate(KDNode *node, Point& P1, Point& P2, std::vector<AABBox*>& bBoxes, unsigned int depth)
{
    unsigned int objectCount = bBoxes.size();
    if(objectCount <= MAXOBJECTS || !depth)
    {
        node->axis = LEAF;
        node->children = NULL;
        node->objects = new std::vector<AABBox*>(bBoxes);
        return;
    }

    Vector relativeBox = P2-P1;
    myFloat totalArea = 2*(relativeBox.x*relativeBox.y+relativeBox.x*relativeBox.z+relativeBox.y*relativeBox.z);
    myFloat invTotalArea = 1.0/totalArea;
    myFloat bestCost = CI*objectCount;
    Axes bestAxis = LEAF;
    myFloat bestSplitPosition = 0.0;

    const Axes otherAxes[3][2] = {{YAXIS, ZAXIS}, {XAXIS, ZAXIS}, {XAXIS, YAXIS}};
    for(int axis = XAXIS; axis<=ZAXIS; axis++)
    {
        // Populate KDSplitElementVector
        std::vector<KDSplitElement> splitElements;
        for(unsigned int i = 0; i<objectCount; i++)
        {
            splitElements.push_back(KDSplitElement((bBoxes[i]->P1)[axis], bBoxes[i], LEFT));
            splitElements.push_back(KDSplitElement((bBoxes[i]->P2)[axis], bBoxes[i], RIGHT));
        }

        // Sort the elements from left to right. If equal, put RIGHT coordinates first.
        std::sort(splitElements.begin(), splitElements.end());

        // Find the lowest splitting cost

        myFloat splitPlaneArea = 2*relativeBox[otherAxes[axis][0]]* relativeBox[otherAxes[axis][1]];
        myFloat unitArea = 2*(relativeBox[otherAxes[axis][0]]+relativeBox[otherAxes[axis][1]]);
        myFloat bestLocalCost = MAX_FLOAT;
        unsigned int bestLocalSplitIndex = 0;
        unsigned int lObjects=0, rObjects = objectCount;

        // Cycle through all splitting positions
        for(unsigned int i=0; i<2*objectCount;i++)
        {
            if(splitElements[i].type == RIGHT)
                rObjects--;
            // Only consider actual splits...
            if(splitElements[i].position > P1[axis] && splitElements[i].position < P2[axis])
            {
                myFloat temp = (splitElements[i].position-P1[axis])*unitArea;
                myFloat lArea = splitPlaneArea+temp;
                myFloat rArea = totalArea-temp;

                // Give bonus EB if the other half is empty
                myFloat coef = (!lObjects || !rObjects)?EB:1.0;
                myFloat total = coef*(lArea*lObjects+rArea*rObjects);
                if(total<bestLocalCost)
                {
                    bestLocalCost = total;
                    bestLocalSplitIndex = i;
                }
            }
            if(splitElements[i].type == LEFT)
                lObjects++;
        }

        // See if the best split from this axis was any good
        if(CI*bestLocalCost*invTotalArea+CT < bestCost)
        {
            bestCost = CI*bestLocalCost*invTotalArea+CT;
            bestAxis = (Axes) axis;
            bestSplitPosition = splitElements[bestLocalSplitIndex].position;
        }
    }
    
    if(bestAxis != LEAF)
    {

        std::vector<AABBox*> leftBoxes;
        std::vector<AABBox*> rightBoxes;

        for(int unsigned i = 0; i<objectCount;i++)
        {
            // If the left edge is inside left node, insert it there.
            if((bBoxes[i]->P1)[bestAxis] < bestSplitPosition)
                leftBoxes.push_back(bBoxes[i]);

            // If the right edge is inside right node, insert it there.
            if((bBoxes[i]->P2)[bestAxis] > bestSplitPosition)
                rightBoxes.push_back(bBoxes[i]);
        }

        if((leftBoxes.size() == objectCount) && (rightBoxes.size() == objectCount))
        {
            node->axis = LEAF;
            node->children = NULL;
            node->objects = new std::vector<AABBox*>(bBoxes);
            return;
        }

        Point P2L(P2), P1R(P1);
        P2L[bestAxis] = P1R[bestAxis] = bestSplitPosition;

        node->children = new KDNode[2]();
        node->axis = bestAxis;
        node->splitPosition = bestSplitPosition;
        populate(node->children, P1, P2L, leftBoxes,  depth-1);
        populate(node->children+1, P1R, P2, rightBoxes,  depth-1);
    }
    else
    {
        node->axis = LEAF;
        node->children = NULL;
        node->objects = new std::vector<AABBox*>(bBoxes);
    }

    return;
}
#else
void KDTree::populate(KDNode *node, Point& P1, Point& P2, std::vector<AABBox*>& objects, unsigned int depth)
{
    // If less objects than MAXOBJECTS, this is leaf
    unsigned int objectCount = objects.size();
    if(objectCount <= MAXOBJECTS || !depth)
    {
        node->axis = LEAF;
        node->children = NULL;
        node->objects = new std::vector<AABBox*>(objects);
        return;
    }

    // Not a leaf node so don't store any object info
    node->objects = NULL;

    // Define splitting axis

    Vector dimension(P2.x - P1.x, P2.y - P1.y, P2.z - P1.z);
    
    Axes axis;
    if(dimension.x >= dimension.y && dimension.x >= dimension.z)
        axis = XAXIS;
    else if(dimension.y >= dimension.z)
        axis = YAXIS;
    else
        axis = ZAXIS;

    myFloat splitPosition = P1[axis] + 0.5*dimension[axis];
    node->splitPosition = splitPosition;
    node->axis = axis;
    
    // Figure out which objects go to which nodes.
    // Now just cut the current node in 2 equal halves
    // and test which bounding boxes remain in which half.
    
    std::vector<AABBox*> leftObjects;
    std::vector<AABBox*> rightObjects;

    for(int unsigned i = 0; i<objectCount;i++)
    {
        // If the left edge is inside left node, insert it there.
        if((objects[i]->P1)[axis] < splitPosition)
            leftObjects.push_back(objects[i]);
        
        // If the right edge is inside right node, insert it there.
        if((objects[i]->P2)[axis] > splitPosition)
            rightObjects.push_back(objects[i]);
    }

    if((leftObjects.size() == objectCount) && (rightObjects.size() == objectCount))
    {
        node->axis = LEAF;
        node->children = NULL;
        node->objects = new std::vector<AABBox*>(objects);
        return;
    }

    Point P2L(P2), P1R(P1);
    P2L[axis] = P1R[axis] = splitPosition;
    
    node->children = new KDNode[2]();
    populate(node->children, P1, P2L, leftObjects,  depth-1);
    populate(node->children+1, P1R, P2, rightObjects,  depth-1);
    return;

}
#endif

bool KDTree::intersect(Ray& ray, const AABBox& bounds, Intersection& iInfo, myFloat minDistance) const
{
    myFloat a=0.0,b=0.0;
    iInfo.distance = MAX_FLOAT;
    iInfo.object = NULL;
    Intersection intersection;
       
    // First scan all non-boundable/infinite objects. Those are not stored in kd-tree.
    for(unsigned int i=0; i < nonBoundableObjects.size(); i++)
    {
        Object *object = nonBoundableObjects[i];

        if(object->intersect(ray, intersection)) 
        {
        // This is for light source probing
            if(intersection.distance < minDistance)
                return false;

            if(intersection.distance < iInfo.distance)
            {
                iInfo.distance = intersection.distance;
                iInfo.object = object;
                iInfo.normal = intersection.normal;
            }
        }
    }

    if(!bounds.intersect(ray, a, b) && !iInfo.object)
        return false;

    if(iInfo.object)
        b = std::min(b, iInfo.distance);

#define STACKDEPTH 64
    KDStackElement stack[STACKDEPTH];
    KDNode *farChild, *currentNode;
    currentNode = root;

    unsigned int entryElement = 0;
    stack[entryElement].distance = a;

    if(a > EPSILON)
        stack[entryElement].point = ray.origin + a*ray.direction;
    else
        stack[entryElement].point = ray.origin;

    unsigned int exitElement = 1;
    stack[exitElement].distance = b;
    stack[exitElement].point = ray.origin + b*ray.direction;
    stack[exitElement].node = NULL;

    while(currentNode != NULL)
    {
        while(currentNode->axis != LEAF)
        {
            Axes axis = currentNode->axis;
            myFloat splitPosition = currentNode->splitPosition;

            Axes nextAxis, previousAxis;
            switch(axis)
            {
                case XAXIS:
                    nextAxis = YAXIS;
                    previousAxis = ZAXIS;
                    break;
                case YAXIS:
                    nextAxis = ZAXIS;
                    previousAxis = XAXIS;
                    break;
                case ZAXIS:
                    nextAxis = XAXIS;
                    previousAxis = YAXIS;
                    break;

                case LEAF:
                default:
                    std::cout<<"KDTree::intersect(): invalid axis!\n";
                    abort();
                    break;
            }    

            if(stack[entryElement].point[axis] <= splitPosition) // Entry point in left or at split
            {
                if(stack[exitElement].point[axis] <= splitPosition) // Exit point also in left or at split. We are interested in left only.
                {
                    currentNode = currentNode->children;
                    continue;
                }
                if(stack[entryElement].point[axis] == splitPosition) // Entry at split and exit in right.
                {
                    currentNode = currentNode->children+1;
                    continue;
                }
                farChild = currentNode->children+1;
                currentNode = currentNode->children;
            }
            else // Entry at right
            {
                if(stack[exitElement].point[axis] >= splitPosition) // Exit at right or split
                {
                    currentNode = currentNode->children+1;
                    continue;
                }

                farChild = currentNode->children;
                currentNode = currentNode->children+1;
            }

            myFloat distance = (splitPosition - ray.origin[axis]) / ray.direction[axis];
            unsigned int temp = exitElement;
            exitElement++;

            if(exitElement == entryElement)
                exitElement++;

            stack[exitElement].previousElement = temp;
            stack[exitElement].distance = distance;
            stack[exitElement].node = farChild;
            stack[exitElement].point[axis] = splitPosition;
            stack[exitElement].point[nextAxis] = ray.origin[nextAxis] + distance*ray.direction[nextAxis];
            stack[exitElement].point[previousAxis] = ray.origin[previousAxis] + distance*ray.direction[previousAxis];
        } // !Leaf node
        // This is leaf. Intersect all objects.
        bool foundSomething = false;
        for(unsigned int i=0; i < currentNode->objects->size(); i++)
        {
            Object *object = ((*(currentNode->objects))[i])->owner;

            if(object->intersect(ray, intersection))
            {
                if(intersection.distance < (stack[entryElement].distance-EPSILON) || intersection.distance>(stack[exitElement].distance+EPSILON))
                    continue;

                // This is for light source probing. Terminate if we found something between us and the light source.
                if(intersection.distance < minDistance)
                    return false;

                if(intersection.distance < iInfo.distance)
                {
                    foundSomething = true;
                    iInfo.distance = intersection.distance;
                    iInfo.object = object;
                    iInfo.normal = intersection.normal;
                }
            }
        } 

        if(foundSomething)
            return true;

        entryElement = exitElement;
        currentNode = stack[exitElement].node;
        exitElement = stack[entryElement].previousElement;
    } // Nodes left

    if(iInfo.object)
        return true;
    else
        return false;
}

void KDTree::destroy(KDNode *node)
{
    // If this is LEAF delete objects vector
    if(node->axis == LEAF)
        delete node->objects;
    else
    {
        // Destroy the children
        destroy(node->children);
        destroy(node->children+1);
        delete[] node->children;
    }
}

unsigned int KDTree::nodeCount() const
{
    return nodeCount(root);
}

unsigned int KDTree::nodeCount(KDNode *node) const
{
    static unsigned int nodes = 0;
    nodes++;
    if(node->axis == LEAF)
        return nodes;
    nodeCount(node->children);
    nodeCount(node->children+1);
    return nodes;
}

unsigned int KDTree::leafCount() const
{
    return leafCount(root);
}

unsigned int KDTree::leafCount(KDNode *node) const
{
    if(node->axis == LEAF)
        return 1;
    else
        return leafCount(node->children) + leafCount(node->children+1);
}

unsigned int KDTree::maxDepth() const
{
    return maxDepth(root, 0);
}
unsigned int KDTree::maxDepth(KDNode *node, unsigned int depth) const
{
    static unsigned int max = 0;
    if(node->axis == LEAF)
    {
        if(depth > max)
            max = depth;
        return max;
    }

    maxDepth(node->children, depth+1);
    maxDepth(node->children+1, depth+1);
    return max;
}

unsigned int KDTree::maxLeafPopulation(KDNode *node) const
{
    static unsigned int max = 0;
    if(node->axis == LEAF)
    {
        if(node->objects->size() > max)
            max = node->objects->size();
    }
    else
    {
        maxLeafPopulation(node->children);
        maxLeafPopulation(node->children+1);
    }
    return max;
}

unsigned int KDTree::maxLeafPopulation() const
{
    return maxLeafPopulation(root);
}
