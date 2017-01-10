#ifndef KDSPLITELEMENT
#define KDSPLITELEMENT

enum splitType {LEFT, RIGHT};

class AABBox;
struct KDSplitElement
{

    myFloat position;
    AABBox *bBox;
    splitType type;

    KDSplitElement(myFloat position, AABBox *bBox, splitType type) : position(position), bBox(bBox), type(type)
    {}

    /* This is necessary for std::sort() to give correct results. If 2 items overlap we want to
     * put the one with RIGHT edge lower than the one with LEFT edge
     */

    bool operator<(const KDSplitElement &kDSplitElement) const
    {
        if(position == kDSplitElement.position)
            return (type > kDSplitElement.type);
        return position < kDSplitElement.position;
    }
};

#endif
