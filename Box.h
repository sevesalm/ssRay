#ifndef BOX_H
#define BOX_H

#include "Vector.h"
#include "Object.h"
#include "Material.h"
#include "Transformation.h"
//#define NONSCALE

class Box : public Object
{
    private:
        Transformation *transformation;

    public:
        Box(Material *, Transformation *);
        ~Box();

        bool intersect(Ray&, Intersection&) const;
        BSphere *boundByBSphere();
        AABBox *boundByAABBox();
};
#endif
