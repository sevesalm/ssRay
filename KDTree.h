#ifndef KDTREE_H
#define KDTREE_H

#include "AABBox.h"
#include "KDNode.h"
#include "Intersection.h"
#include <vector>
class KDTree
{
    private:
        KDNode *root;
        std::vector<Object*> nonBoundableObjects;
        void populate(KDNode *, Point& P1, Point& P2, std::vector<AABBox*>&, unsigned int);
        void destroy(KDNode *);
        unsigned int leafCount(KDNode *) const;
        unsigned int nodeCount(KDNode *) const;
        unsigned int maxDepth(KDNode *, unsigned int) const;
        unsigned int maxLeafPopulation(KDNode *) const;

    public:
        KDTree(std::vector<AABBox*>&, std::vector<Object*>&, AABBox&);
        ~KDTree();
        
        bool intersect(Ray&, const AABBox&, Intersection&, myFloat = MIN_FLOAT) const;
        
        unsigned int leafCount() const;
        unsigned int nodeCount() const;
        unsigned int maxDepth() const;
        unsigned int maxLeafPopulation() const;
};

#endif
