#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Vector.h"
#include "Point.h"
#include "Object.h"
#include "Material.h"

class Triangle : public Object
{
    private:
        Point origin;
        Vector normal;
        Vector edge1,edge2;

    public:
        Triangle(Material *, Point, Point, Point);
        AABBox *boundByAABBox();
        BSphere *boundByBSphere();
        bool intersect(Ray&, Intersection&) const;
};
#endif
