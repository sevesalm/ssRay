#ifndef OBJECT_H
#define OBJECT_H

#include "AABBox.h"
#include "BSphere.h"
#include "Random.h"
#include "Vector.h"
#include "Point.h"
#include "Material.h"
#include "Transformation.h"
#include "Intersection.h"
#include <iostream>
#include <vector>
class Object
{
    private:

    protected:
        BSphere *bSphere;
        AABBox *aABBox;

    public:
        Material *material;
  
        Object(Material *material) :  bSphere(NULL), aABBox(NULL), material(material)
        { };

        virtual ~Object() 
        {
            delete bSphere;
            delete aABBox;
        };

        virtual bool intersect(Ray&, Intersection&) const =0;
        virtual Vector sampleLight(Point&, Random&) const {return Vector(0);}
        virtual myFloat getSolidRatio(Point&) const {return 0.0;}
        virtual BSphere *boundByBSphere() { return NULL; }
        virtual AABBox *boundByAABBox() { return NULL; }
 
        BSphere *getBSphere() const
        {
            return bSphere;
        }

        AABBox *getAABBox() const
        {
            return aABBox;
        }
};

#endif
