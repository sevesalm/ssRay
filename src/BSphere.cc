#include "BSphere.h"
#include <cmath>

BSphere::BSphere(Point position, myFloat radius) : position(position), radius(radius), radius2(radius*radius)
{}

// Return the solid ratio of the sphere as seen from target.
myFloat BSphere::getSolidRatio(Point& target) const
{   
    Vector offset = target-position; 
    myFloat d2 = offset.length2();
    if( d2 <= radius2+EPSILON)
        return 1.0;

    return (1-sqrt(1-radius2/d2));
}

Vector BSphere::createRandomRay(const Point& target, Random& rNG, myFloat y) const 
{
    Vector direction = (position-target).unit();
    y = 1-y*(1-rNG.randomNumberClosed());
    myFloat r = sqrt(1-y*y);
    myFloat phi = 2 * M_PI * rNG.randomNumberClosed();
    myFloat x = r * cos(phi);
    myFloat z = r * sin(phi);
    
    return direction.orientY(Vector(x,y,z));
}

