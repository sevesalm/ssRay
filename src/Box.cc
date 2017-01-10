#include "Box.h"
#include "utils.h"
#include <iostream>

/* Box is axis aligned between points P1 (0,0,0) and P2 (1,1,1). This makes it possible to optimize the intersection routine. */

Box::Box(Material *material, Transformation * transformation) : Object(material), transformation(transformation)
{
    transformation->createInverse();
}

Box::~Box()
{
    delete transformation;
}

enum NormalType {XMIN, XMAX, YMIN, YMAX, ZMIN, ZMAX};

bool Box::intersect(Ray& ray, Intersection& intersection) const
{
    Vector normalList[6] = {Vector(-1,0,0), Vector(1,0,0), Vector(0,-1,0), Vector(0,1,0), Vector(0,0,-1), Vector(0,0,1)};
    NormalType nt[6] = {XMIN, XMAX, YMIN, YMAX, ZMIN, ZMAX};
    Point origin = transformation->worldToObject(ray.origin);
    Vector direction = transformation->worldToObject(ray.direction).unit();

    myFloat tmin, tmax, tymin, tymax, tzmin, tzmax;

    myFloat divx = 1.0/direction.x;
    if (divx >= 0.0) {
        tmin = -origin.x*divx;
        tmax = tmin+divx;
    }
    else {
        nt[0]=XMAX;
        nt[1]=XMIN;
        tmax =  -origin.x *divx;
        tmin = tmax + divx;
    }

    myFloat divy = 1.0/direction.y;
    if (direction.y >= 0) {
        tymin = -origin.y *divy;
        tymax = tymin+divy;
    }
    else {
        nt[2]=YMAX;
        nt[3]=YMIN;
        tymax = -origin.y *divy;
        tymin = tymax+divy;
    }

    if ( (tmin > tymax) || (tymin > tmax) )
        return false;
    NormalType normalType[2];
    
    if (tymin > tmin)
    {
        tmin = tymin;
        normalType[0]=  nt[2];
    }
    else
        normalType[0] = nt[0];

    if (tymax < tmax)
    {
        tmax = tymax;
        normalType[1]= nt[3];
    }
    else
        normalType[1] = nt[1];

    myFloat divz = 1.0/direction.z;
    if (direction.z >= 0) {
        tzmin = -origin.z *divz;
        tzmax = tzmin+divz;
    }
    else {
        nt[4]=ZMAX;
        nt[5]=ZMIN;
        tzmax = -origin.z *divz;
        tzmin = tzmax+divz;
    }
    if ( (tmin > tzmax) || (tzmin > tmax) )
        return false;
    
    if (tzmin > tmin)
    {
        tmin = tzmin;
        normalType[0]=nt[4];
    }
    if (tzmax < tmax)
    {
        tmax = tzmax;
        normalType[1]=nt[5];
    }

    myFloat distance;
    unsigned int nType;
    if(tmin > EPSILON)
    {
        distance = tmin;
        nType = 0;
    }
    else 
    {
        if((distance = tmax) < EPSILON)
            return false;
        else
            nType = 1;
    }
    
    intersection.distance = (transformation->objectToWorld(origin + direction*distance)-ray.origin).length();
    intersection.normal = transformation->objectNormalToWorld(normalList[normalType[nType]]);
    return true;
}

//FIX to allow transformations. Then bound by checking all lävistäjät.
BSphere *Box::boundByBSphere()
{
    Point P1 = transformation->objectToWorld(Point(0,0,0));
    Point P2 = transformation->objectToWorld(Point(1,1,1));
    
    Point origin = P1 + (P2-P1)/2.0;
    myFloat radius = (P2-P1).length()/2.0;

    bSphere= new BSphere(origin, radius);
    return bSphere;
}

AABBox *Box::boundByAABBox()
{
    Point p[8];
    p[0] = transformation->objectToWorld(Point(0,0,0));
    p[1] = transformation->objectToWorld(Point(1,0,0));
    p[2] = transformation->objectToWorld(Point(1,0,1));
    p[3] = transformation->objectToWorld(Point(0,0,1));
    p[4] = transformation->objectToWorld(Point(0,1,0));
    p[5] = transformation->objectToWorld(Point(1,1,0));
    p[6] = transformation->objectToWorld(Point(1,1,1));
    p[7] = transformation->objectToWorld(Point(0,1,1));
   
   aABBox = new AABBox(p[0], p[0], this);
   for(unsigned int i=1; i<8; i++)
       aABBox->include(p[i]);

   //aABBox->addEpsilon();
   return aABBox;

}

