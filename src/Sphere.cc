#include "Sphere.h"
#include <sstream>
#include <cmath>


/* Sphere is always centered at origin with radius = 1. This is because transformation matrix handles the actual placement and scale. Makes it faster to calculate intersections etc.*/

Sphere::Sphere(Material *material, Transformation *transformation) : Object(material), transformation(transformation)
{
    transformation->createInverse();
}

Sphere::~Sphere()
{
    delete transformation;
}

bool Sphere::intersect(Ray& ray, Intersection& intersection) const
{
    Point origin = transformation->worldToObject(ray.origin);
    Vector direction = transformation->worldToObject(ray.direction).unit(); 

    Vector relPosition = Vector(-origin.x, -origin.y, -origin.z);
    myFloat nearestPointDistance = relPosition.dot(direction);

    myFloat halfChord2 = 1.0 - relPosition.dot(relPosition) + nearestPointDistance*nearestPointDistance;

    if(halfChord2 > 0.0)
    {
        const myFloat halfChord = sqrt(halfChord2);

        myFloat a = nearestPointDistance - halfChord;
        myFloat distance;
        if(a > EPSILON)
            distance = a;
        else if((distance = nearestPointDistance + halfChord) < EPSILON)
            return false;

        // FIX!!!! Allow explicit conversion from point to vector!
        Point localHitPoint = origin + direction*distance;
        Vector localNormal = Vector(localHitPoint.x, localHitPoint.y, localHitPoint.z);
        intersection.normal = transformation->objectNormalToWorld(localNormal);
        intersection.distance = (transformation->objectToWorld(localHitPoint)-ray.origin).length();
        return true;
    }
    else
        return false;
}

/*Vector Sphere::getINormal(Point worldPoint) const
{
    Point localPoint = transformation->worldToObject(worldPoint);
    Vector normal = Vector(localPoint.x, localPoint.y, localPoint.z);
    return transformation->objectNormalToWorld(normal);
}*/

BSphere *Sphere::boundByBSphere()
{
    Point origin = transformation->objectToWorld(Point(0,0,0));
    myFloat radius = (transformation->objectToWorld(Point(0,1,0))-origin).length();

    bSphere= new BSphere(origin, radius);
    return bSphere;
}

AABBox *Sphere::boundByAABBox()
{
    Point center = transformation->objectToWorld(Point(0));
    myFloat radius = ((transformation->objectToWorld(Point(0,1,0)))-center).length();
    aABBox = new AABBox(center-radius*Vector(1), center+radius*Vector(1), this);
    return aABBox;
}
