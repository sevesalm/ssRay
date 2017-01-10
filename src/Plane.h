#ifndef PLANE_H
#define PLANE_H

#include "Vector.h"
#include "Point.h"
#include "Object.h"
#include "Material.h"
#include "Transformation.h"

class Plane : public Object
{
    private:
        Point origin;
        Vector normal;

    public:
        Plane(Point, Vector, Material *);
        ~Plane();

        bool intersect(Ray&, Intersection&) const;
};
#endif
