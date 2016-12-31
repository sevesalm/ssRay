#include "Triangle.h"
#include "utils.h"

/*
 * Triangle is always in world space so no transformations are necessary.
 * Now these are stored straight in object list. Later I might want to use meshes
 * and store only pointer to the 1st vertex in mesh and save memory. We'll see...
 * */

Triangle::Triangle(Material *material, Point v0, Point v1, Point v2) : Object(material)
{
    if(v0 == v1 || v0 == v2 || v1 == v2)
    {
        std::cout<<"Triangle::Triangle(): Needs 3 different vertices: "<<v0.toString()<<", "<<v1.toString()<<", "<<v2.toString()<<"\n";
        abort();
    }
    origin = v0;
    edge1 = v1-v0;
    edge2 = v2-v0;
    normal = (edge1.cross(edge2)).unit();

    return;
}

AABBox *Triangle::boundByAABBox()
{
    myFloat xMin = std::min(origin.x, (origin+edge1).x);
    xMin = std::min(xMin, (origin+edge2).x);
    myFloat yMin = std::min(origin.y, (origin+edge1).y);
    yMin = std::min(yMin, (origin+edge2).y);
    myFloat zMin = std::min(origin.z, (origin+edge1).z);
    zMin = std::min(zMin, (origin+edge2).z);

    myFloat xMax = std::max(origin.x, (origin+edge1).x);
    xMax = std::max(xMax, (origin+edge2).x);
    myFloat yMax = std::max(origin.y, (origin+edge1).y);
    yMax = std::max(yMax, (origin+edge2).y);
    myFloat zMax = std::max(origin.z, (origin+edge1).z);
    zMax = std::max(zMax, (origin+edge2).z);

    if(xMin==xMax)
    {
        xMax+=EPSILON;
        xMin-=EPSILON;
    }
    if(yMin==yMax)
    {
        yMax+=EPSILON;
        yMin-=EPSILON;
    }
    if(zMin==zMax)
    {
        zMax+=EPSILON;
        zMin-=EPSILON;
    }
    aABBox = new AABBox(Point(xMin, yMin, zMin), Point(xMax, yMax, zMax), this);
    return aABBox;
}   

BSphere *Triangle::boundByBSphere()
{
   Point center = aABBox->P1+0.5*(aABBox->P2-aABBox->P1);
   bSphere = new BSphere(center, (aABBox->P2-center).length());
   return bSphere; 
}

bool Triangle::intersect(Ray& ray, Intersection& intersection) const
{
    Vector s1 = ray.direction.cross(edge2);
    myFloat divisor = s1.dot(edge1);
    if(divisor==0.0)
        return false;
    myFloat invDivisor= 1.0 /divisor;

    Vector d = ray.origin - origin;
    myFloat b1 = d.dot(s1) * invDivisor;
    if(b1<0.0 || b1>1.0)
        return false;

    Vector s2 = d.cross(edge1);
    myFloat b2 = ray.direction.dot(s2)*invDivisor;
    if(b2<0.0 || b1+b2>1.0)
        return false;
   
    if((intersection.distance = edge2.dot(s2) * invDivisor) < EPSILON)
        return false;

    intersection.normal = normal;
    return true;
    //if(t<ray.mint||t>ray.maxt)
    //return -1.0;
}

