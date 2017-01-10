#ifndef KDSTACKELEMENT
#define KDSTACKELEMENT
struct KDStackElement
{
    KDNode *node;
    myFloat distance;
    Point point;
    unsigned int previousElement;
};
#endif
