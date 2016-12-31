#ifndef KDNODE_H
#define KDNODE_H

#include "AABBox.h"
#include <vector>
enum Axes {XAXIS, YAXIS, ZAXIS, LEAF};

struct KDNode
{
    Axes axis; //4 bytes
    myFloat splitPosition; //4 bytes 
    // These can be stored in an union as only one is needed
    union
    {
        std::vector<AABBox*> *objects ; //8bytes
        struct KDNode *children; //8bytes
    };
};

#endif
