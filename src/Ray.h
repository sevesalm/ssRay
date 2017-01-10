#ifndef RAY_H
#define RAY_H
#include "Point.h"
#include "Vector.h"
class Ray
{
    private:
    public:
        Point origin;
        Vector direction;
        myFloat min;
        myFloat max;

        Ray(Point origin, Vector direction) : origin(origin), direction(direction), min(0.0), max(MAX_FLOAT)
        {
        }
};
#endif
