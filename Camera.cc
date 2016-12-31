#include "Camera.h"
#include "Random.h"
#include <iostream>

Camera::Camera(Point position, Point target, myFloat ang, unsigned int width, unsigned int height, myFloat apertureRadius, myFloat fDistance) : film(width, height), position(position), sky(0, 1, 0), apertureRadius(apertureRadius)
{
    if(target == position)
    {
        std::cout<<"Camera.c++: Camera::Camera(): target == position!!\n";
        abort();
    }

    direction = (target-position).unit();
    
    right = (sky.cross(direction)).unit();
    up = (direction.cross(right)).unit() * ((myFloat) film.getHeight()/film.getWidth());
    angle = ang*M_PI/180.0;
    direction /= (2 * tan(angle/2));
    if(fDistance == 0.0)
        focalDistance = (target-position).length();
    else
        focalDistance = fDistance;

    return;
}

//Create initial ray. Antialiasing is performed using a single random ray through the pixel.
Ray Camera::createInitialRay(unsigned int x, unsigned int y, Random &rNG) const 
{
    myFloat x0 = (x + rNG.randomNumberClosed())/film.getWidth() - 0.5;
    myFloat y0 = 0.5 - (y + rNG.randomNumberClosed())/film.getHeight();

    Vector initialDirection = (direction + right*x0 + up*y0).unit();

    //Use a quicker method if apertureRadius is 0.0 -> no DOF
    if(apertureRadius > 0.0)
    {
        Point focalPoint = position + initialDirection*(focalDistance/(initialDirection.dot(direction.unit()))); 

        //Sample a random point on a circular lens
        myFloat lensX,lensY;
        do{
            lensX = 2.0 * rNG.randomNumberClosed() - 1.0;
            lensY = 2.0 * rNG.randomNumberClosed() - 1.0;
        }
        while(lensX*lensX + lensY*lensY > 1.0);

        Point lensPosition = position + apertureRadius*(right*lensX + up.unit()*lensY);

        return Ray(lensPosition, (focalPoint - lensPosition).unit());
    }
    else
        return Ray(position, initialDirection);
}


Film *Camera::getFilm(void)
{
    return &film;
}
