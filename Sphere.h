#ifndef SPHERE_H
#define SPHERE_H
#include "Vector.h"
#include "Material.h"
#include "Object.h"
#include "Random.h"
#include "Transformation.h"
#include <iostream>

class Sphere : public Object
{
    private:
        Transformation *transformation;

    public:
        Sphere(Material *, Transformation *);
        ~Sphere();
        
        bool intersect(Ray&, Intersection&) const;
        BSphere *boundByBSphere();
        AABBox *boundByAABBox();

};
#endif
