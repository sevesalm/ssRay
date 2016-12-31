#ifndef BSPHERE_H
#define BSPHERE_H

#include "Point.h"
#include "Vector.h"
#include "Random.h"

class BSphere
{
    private:
        Point position;
        myFloat radius, radius2;

    public:
        BSphere(Point, myFloat);
        myFloat getSolidRatio(Point&) const;
        Vector createRandomRay(const Point&, Random&, myFloat) const;
};

#endif
