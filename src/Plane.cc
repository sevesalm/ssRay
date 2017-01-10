#include "Plane.h"

/* Plane is oriented in world space and transformations to local space is not needed. */

Plane::Plane(Point origin, Vector normal, Material *material) : Object(material), origin(origin), normal(normal) 
{
}

Plane::~Plane()
{
}

bool Plane::intersect(Ray& ray, Intersection& intersection) const
{

    myFloat ND = ray.direction.dot(normal);

    if(ND >= 0.0)
        return false;

    Vector rel = origin-ray.origin;
    myFloat distance = normal.dot(rel)/ND;
    
    if(distance > EPSILON)
    {
        intersection.normal = normal;
        intersection.distance = distance;
        return true;
    }
    else 
        return false;
}

