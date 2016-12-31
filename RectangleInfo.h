#ifndef RECTANGLEINFO_H
#define RECTANGLEINFO_H
struct RectangleInfo
{
    bool fullScreen;
    bool catchUp;
    unsigned int xstart, ystart, xend, yend;

    bool operator==(const RectangleInfo& rectangle) const
    {
        return(xstart==rectangle.xstart && ystart==rectangle.ystart && xend==rectangle.xend && yend==rectangle.yend);
    }

    bool operator!=(const RectangleInfo& rectangle) const
    {
        return !(*this == rectangle);
    }
};
#endif
