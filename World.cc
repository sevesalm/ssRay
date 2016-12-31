#include "World.h"
#include "Intersection.h"
#include "Ray.h"
#include "Matrix.h"
#include "SkyDome.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Triangle.h"
#include "KDTree.h"
#include <fstream>
#include <iomanip>

#define DIRECTLIGHTING

//The number of threads the program launches when updating world. Number of cores is a good value.

World::World() : bounds(Point(MAX_FLOAT), Point(-MAX_FLOAT), NULL), kDTree(NULL)
{
}

World::~World()
{
    for(unsigned int i=0; i<objectVector.size();i++)
        delete objectVector[i];
    delete kDTree;
}

void World::initialize()
{
    unsigned int bSphereCount=0, lightCount=0;
    std::vector<AABBox*> bBoxVector;
    std::vector<Object*> nonBoundableObjects;
    // Perform initialization.
    std::cout<<"\nInitializing World:\n";

    std::cout<<"    Objects: "<<objectCount()<<"\n";
    std::cout<<"    Bounding objects "<<std::flush;
    for(unsigned int i=0; i<objectCount(); i++)
    {
        Object *object = objectVector[i];
        AABBox *aABBox;
        aABBox = object->boundByAABBox();
        if(aABBox)
        {
            // Store it so KDTree can use the vector to build the tree.
            bBoxVector.push_back(aABBox);
            if(aABBox->P1.x<bounds.P1.x)
                bounds.P1.x = aABBox->P1.x;
            if(aABBox->P1.y<bounds.P1.y)
                bounds.P1.y = aABBox->P1.y;
            if(aABBox->P1.z<bounds.P1.z)
                bounds.P1.z = aABBox->P1.z;
            if(aABBox->P2.x>bounds.P2.x)
                bounds.P2.x = aABBox->P2.x;
            if(aABBox->P2.y>bounds.P2.y)
                bounds.P2.y = aABBox->P2.y;
            if(aABBox->P2.z>bounds.P2.z)
                bounds.P2.z = aABBox->P2.z;
        }
        else
            nonBoundableObjects.push_back(object);
        if(object->material->isEmitting())
        {
            lightCount++;
            BSphere *bSphere;
            bSphere = object->boundByBSphere();
            if(bSphere)
            {
                lightVector.push_back(object);
                bSphereCount++;
            }
        }
    }
    std::cout<<"[OK]\n";
    std::cout<<"        Light sources: "<<lightCount<<'\n';
    std::cout<<"        BSpheres: "<<bSphereCount<<'\n';
    std::cout<<"        AABBoxes: "<<bBoxVector.size()<<'\n';
    std::cout<<"        Infinite: "<<nonBoundableObjects.size()<<'\n';
    std::cout<<"        World bounds: "<<bounds.P1.toString()<<", "<<bounds.P2.toString()<<'\n';

    std::cout<<"    Building KDTree "<<std::flush;
    double startTime = getTime();
    kDTree = new KDTree(bBoxVector, nonBoundableObjects, bounds);
    unsigned int nodeCount = kDTree->nodeCount();
    unsigned int leafCount = kDTree->leafCount();
    std::cout<<"[OK] ("<<std::setprecision(2)<<getTime()-startTime<<"s)\n";
    std::cout<<"        Nodes: "<<nodeCount<< " (interior: "<<nodeCount-leafCount<<", leafs: "<<leafCount<<")\n";
    std::cout<<"        Max depth: "<<kDTree->maxDepth()<<'\n';
    std::cout<<"        Max leaf population: "<<kDTree->maxLeafPopulation()<<'\n';

    std::cout<<"Initialization complete\n\n";
}

void World::addTriangle(Point v0, Point v1, Point v2, Material *material, Transformation *transformation)
{
    if(transformation)
    {
        v0 = transformation->objectToWorld(v0);
        v1 = transformation->objectToWorld(v1);
        v2 = transformation->objectToWorld(v2);
    }
    Object *object = new Triangle(material, v0,v1,v2);
    objectVector.push_back(object);

}

/* Read .obj file, parse it and store as triangles. 
 */
 
void World::addMesh(std::string fileName, Material *material, Transformation *transformation, bool invert)
{ 
    std::ifstream myFile(fileName.c_str(), std::ios::in);
    if (myFile.is_open())
    {
        std::vector<Point> vertices;
        std::string line;
        unsigned int triangleCount = 0, trianglesSkipped = 0;
        
        /* Read line by line
         * v = vertex info, store vertices
         * f = face info, store triangles
         */

        std::cout<<"Importing mesh file: "<<fileName<<' '<<std::flush;

        double startTime = getTime();
        getline(myFile, line);
        while(myFile)
        {
            std::istringstream lineStream(line);
            char command;
            lineStream >> command;
            Point p;
            switch(command)
            {
                case 'v':
                    float x,y,z;
                    lineStream>>x;
                    lineStream>>y;
                    lineStream>>z;
                    // Invert z because right-handed coordinates.
                    p = Point(x,y,-z);
                    if(transformation)
                        p = transformation->objectToWorld(p);
                    vertices.push_back(p);
                    break;
                case 'f':
                    unsigned int v0,v1,v2;
                    // This is the order because .obj uses right-handed coordinates. Use invert to invert normals.
                    lineStream>>v0;
                    if(invert)
                    {
                        lineStream>>v1;
                        lineStream>>v2;
                    }
                    else
                    {
                        lineStream>>v2;
                        lineStream>>v1;
                    }
                    if(v0>vertices.size() || v1>vertices.size() || v2>vertices.size())
                    {
                        std::cout<<"World::addMesh(): Invalid .obj file\n";
                        return;
                    }
                    if(vertices[v0-1]== vertices[v1-1] || vertices[v0-1]==vertices[v2-1] || vertices[v1-1]==vertices[v2-1])
                    {
                        trianglesSkipped++;
                        break;
                    }
                    Object *object = new Triangle(material, vertices[v0-1], vertices[v1-1], vertices[v2-1]);
                    objectVector.push_back(object);
                    triangleCount++;
                    if(material->isEmitting())
                        lightVector.push_back(object);
                    break;
            }
            getline(myFile, line);
        }
        std::cout<<"[OK] ("<<std::setprecision(2)<<getTime()-startTime<<"s)\n";
        std::cout<<"    Triangles: "<<triangleCount<<'\n';
        if(trianglesSkipped)
            std::cout<<"    Triangles skipped: "<<trianglesSkipped<<'\n';
    }
    else
        std::cout<<"World::addMesh(): Unable to open file: " << fileName << '\n';
}

void World::addSphere(Point position, myFloat radius, Material *material, Transformation *transformation)
{
    Transformation *basic = new Transformation;
    basic->Scale(Vector(radius));
    basic->Translate(Vector(position.x, position.y, position.z));

    if(transformation)
        basic->Transform(transformation);

    Object *object = new Sphere(material, basic);
    objectVector.push_back(object);

}

void World::addBox(Point P1, Point P2, Material *material, Transformation *transformation)
{
    //#define TRIANGLEBOX
#ifdef TRIANGLEBOX
    Point p1 = P1;
    Point p2 = Point(P2.x, P1.y, P1.z);
    Point p3 = Point(P2.x, P1.y, P2.z);
    Point p4 = Point(P1.x, P1.y, P2.z);
    Point p5 = P2;
    Point p6 = Point(P1.x, P2.y, P2.z);
    Point p7 = Point(P1.x, P2.y, P1.z);
    Point p8 = Point(P2.x, P2.y, P1.z);
    
    addTriangle(p1, p3, p2, material, transformation);
    addTriangle(p1, p4, p3, material, transformation);
    addTriangle(p1, p8, p2, material, transformation);
    addTriangle(p1, p7, p8, material, transformation);
    addTriangle(p4, p7, p1, material, transformation);
    addTriangle(p4, p6, p7, material, transformation);
    addTriangle(p2, p5, p3, material, transformation);
    addTriangle(p2, p8, p5, material, transformation);
    addTriangle(p3, p6, p4, material, transformation);
    addTriangle(p3, p5, p6, material, transformation);
    addTriangle(p5, p7, p6, material, transformation);
    addTriangle(p5, p8, p7, material, transformation);
    return;
#endif
    Transformation *basic = new Transformation;
    basic->Scale(P2 - P1);
    basic->Translate( P1-Point(0,0,0));
    if(transformation)
        basic->Transform(transformation);

    Object *object = new Box(material, basic);

    objectVector.push_back(object);
}

void World::addPlane(Point origin, Vector normal, Material *material, Transformation *transformation)
{
    if(transformation)
    {
        transformation->createInverse();
        origin = transformation->objectToWorld(origin);
        normal = transformation->objectNormalToWorld(normal);
    }
    
    Object *object = new Plane(origin, normal, material);
    objectVector.push_back(object);
}

unsigned int World::lightCount() const
{
    return lightVector.size();
}

unsigned int World::objectCount() const
{
    return objectVector.size();
}

Vector createBiasRay(Random& rNG, myFloat y) 
{
    y = pow(rNG.randomNumberClosed(), 1.0/y);
    myFloat r = sqrt(1-y*y);
    myFloat phi = 2 * M_PI * rNG.randomNumberClosed();
    myFloat x = r * cos(phi);
    myFloat z = r * sin(phi);
    
    return Vector(x,y,z);    
}
Vector World::trace(Ray ray, Random& rNG) const
{
    Vector radiance = Vector(0.0);
    Vector colorMulti = Vector(1.0); // This is altered whenever we hit something "colorful"
    bool lightsEvaluated = false; // 
    myFloat scalar = 1.0;

    Intersection bestIntersection, tempIntersection;
    while(true)
    {
        if(!kDTree->intersect(ray, bounds, bestIntersection))
        {
            radiance += skyDome.getRadiance(ray.direction).combine(colorMulti)*scalar;
            break;
        }

        Object *object = bestIntersection.object;
        ray.origin += ray.direction*bestIntersection.distance;

        //Let's return if this is a light source. 
        if(object->material->isEmitting())
        {
            if(!lightsEvaluated || !object->getBSphere())
                radiance += (object->material->getColor(ray.origin).combine(colorMulti))*scalar;
            break;
        }

#define ABSO 1.1
        myFloat R1 = rNG.randomNumberClosed()*ABSO;
        if(R1 > 1.0)
            break;
        scalar*=ABSO;

        colorMulti = colorMulti.combine(object->material->getColor(ray.origin));

        Vector normal = bestIntersection.normal;
        lightsEvaluated = false;

        if(R1 < object->material->getSpecularCoef())
            ray.direction = createSpecularRay(normal, ray.direction);
            //ray.direction = createGlossyRay(normal, ray.direction, rNG, object->material->getGlossiness());
        else if(R1 < object->material->getRefractiveCoef())
            ray.direction = createRefractiveRay(normal, ray.direction, 1.0, object->material->getRefractiveIndex());
        else
        {
            //Evaluate direct lighting component for very fast convergence.
#ifdef DIRECTLIGHTING     

            for(unsigned int lightIndex=0; lightIndex<lightCount(); lightIndex++)
            {
                const Object *light = lightVector[lightIndex];
                const BSphere *bSphere = light->getBSphere();
                myFloat solidRatio = bSphere->getSolidRatio(ray.origin);
                Vector sampleDirection = bSphere->createRandomRay(ray.origin, rNG, solidRatio);
                Ray lightRay(ray.origin, sampleDirection);
                myFloat lightCos = sampleDirection.dot(normal);
                if(lightCos > 0.0)
                {
                    if(light->intersect(lightRay, tempIntersection))
                    {
                        myFloat maxDistance = tempIntersection.distance;
                        if(kDTree->intersect(lightRay, bounds, tempIntersection, maxDistance))
                        {
                            Point lightPoint = (Point) sampleDirection*maxDistance;
                            radiance += (2*lightCos*solidRatio*light->material->getColor(lightPoint).combine(colorMulti))*scalar;
                        }
                    }
                }
            }
            lightsEvaluated = true;
#endif
            ray.direction = createRandomRay(normal, rNG);
        }
    }
    return radiance;
}

// Create a new ray from a perfectly specular material - ie. a perfect mirror.
Vector World::createSpecularRay(const Vector& normal, const Vector& direction) const
{
    return direction - 2*((normal.dot(direction))*normal);
}



//3 possible glossy reflection models...A is broken!
#define MB
#ifdef MA
Vector World::createGlossyRay(const Vector& normal, const Vector& direction, Random& rNG, myFloat glossiness) const
{
    // Create bias ray
    Vector bias = createBiasRay(rNG, glossiness);
    
    // Combine the two
    // Verify that the object normal is not pointing to sky...
    Vector finalNormal = normal.orientY(bias);

    if(direction.dot(finalNormal)>0.0)
        finalNormal = -finalNormal + 2* (finalNormal.dot(normal)*normal);
//        finalNormal = finalNormal - 2* (direction.dot(finalNormal)*direction);
    
    return direction - 2*((finalNormal.dot(direction))*finalNormal);
    // If the resulting ray is pointing towards the surface, let's invert it.
}
#endif
#ifdef MB
Vector World::createGlossyRay(const Vector& normal, const Vector& direction, Random& rNG, myFloat glossiness) const
{
    // Create specular reay without glossiness
    Vector specular = direction - 2*((normal.dot(direction))*normal);

    // Create bias ray
    Vector bias = createBiasRay(rNG, glossiness);
    
    // Combine the two
    // Verify that the object normal is not pointing to sky...
    Vector final = specular.orientY(bias);
    
    // If the resulting ray is pointing towards the surface, let's invert it.
    if(final.dot(normal)>0.0)
        return final;
    else
        return final - 2*((normal.dot(final))*normal);
}
#endif


/* Create a ray if the material is refractive (ie. glass etc). Using Snell's law. 
 * In special case we must also check for total internal refraction. 
 * */
Vector World::createRefractiveRay(Vector normal, const Vector& direction, myFloat n1, myFloat n2) const
{
    myFloat ratio, c1, cs2;

    c1 = direction.dot(normal);
    if(c1 < 0.0)
    {
        c1 = -c1;
        ratio = n1/n2;
    }
    else
    {
        ratio = n2/n1;
        normal = -normal;
    }

    cs2 = 1.0 - ratio*ratio*(1.0 - c1*c1); 
    if (cs2 < 0.0)
        return createSpecularRay(normal, direction); 
   
    return (ratio*direction + (ratio*c1-sqrt(cs2))*normal);
}

Vector World::createRandomRay(const Vector& normal, Random& rNG) const 
{
#define M3
#ifdef M1
    //SLOWEST, DON'T USE!!
    myFloat y = sqrt(rNG[tIndex].randomNumber());
    myFloat theta = acos(y);
    myFloat phi = 2 * M_PI * rNG[tIndex].randomNumber();

    myFloat x = sin(theta)*cos(phi);
    myFloat z = sin(theta)*sin(phi);
#endif
#ifdef M2
    //Slightly slower than M3
    myFloat y = rNG.randomNumberClosed(); 
    myFloat r = sqrt(1-y*y);
    myFloat phi = 2 * M_PI * rNG.randomNumberClosed();
    myFloat x = r * cos(phi);
    myFloat z = r * sin(phi);
#endif
#ifdef M3
    // Generate a random point on a unit disc. 
    myFloat x,z, temp;

    do{
        x = 2.0 * rNG.randomNumberClosed() - 1.0;
        z = 2.0 * rNG.randomNumberClosed() - 1.0;
        temp = 1.0 - (x*x + z*z);
    }
    while(temp < 0.0);
    
    myFloat y = sqrt(temp); 
#endif
    return normal.orientY(Vector(x,y,z));
}

