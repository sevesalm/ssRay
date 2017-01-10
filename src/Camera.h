#ifndef CAMERA_H
#define CAMERA_H
#include "Vector.h"
#include "Point.h"
#include "Film.h"
#include "Ray.h"
class Random;
class Camera 
{
    private:
        Film film;

        Point position;
        Vector direction;
        Vector sky;
        Vector up;
        Vector right;
        myFloat angle;
        myFloat apertureRadius;
        myFloat focalDistance;
         
    public:
        Camera(Point, Point, myFloat, unsigned int, unsigned int, myFloat = 0.0, myFloat = 0.0);
        Ray createInitialRay(unsigned int, unsigned int, Random&) const;
        Film *getFilm(void);
};
#endif
