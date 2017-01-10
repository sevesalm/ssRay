#ifndef AABBOX_H
#define AABBOX_H

#include "Point.h"
#include "Ray.h"
class Object;
class AABBox
{
    private:
    public:
        Object *owner;
        AABBox(Point p1, Point p2, Object *owner) : owner(owner),  P1(p1), P2(p2) {};
        Point P1,P2;

        void addEpsilon()
        {
            P1 -= EPSILON*(Vector(1,1,1));
            P2 += EPSILON*(Vector(1,1,1));
        }

        void include(Point p)
        {
            for(unsigned int i=0; i<3; i++)
            {
               if(p[i] < P1[i])
                   P1[i] = p[i];
               if(p[i] > P2[i])
                   P2[i] = p[i];
            }
        }

        bool intersect(Ray& ray, myFloat& a, myFloat& b) const
        {
            myFloat tmin, tmax, tymin, tymax, tzmin, tzmax;
            Vector direction = ray.direction;
            Point origin = ray.origin;
        
            myFloat divx = 1.0/direction.x;
            if (divx >= 0.0) {
                tmin = (P1.x - origin.x) *divx;
                tmax = (P2.x - origin.x) *divx;
            }
            else {
                tmin = (P2.x - origin.x) *divx;
                tmax = (P1.x - origin.x) *divx;
            }

            myFloat divy = 1.0/direction.y;
            if (direction.y >= 0) {
                tymin = (P1.y - origin.y) *divy;
                tymax = (P2.y - origin.y) *divy;
            }
            else {
                tymin = (P2.y - origin.y) *divy;
                tymax = (P1.y - origin.y) *divy;
            }
            if ( (tmin > tymax) || (tymin > tmax) )
                return -1.0;
            if (tymin > tmin)
                tmin = tymin;
            if (tymax < tmax)
                tmax = tymax;

            myFloat divz = 1.0/direction.z;
            if (direction.z >= 0) {
                tzmin = (P1.z - origin.z) *divz;
                tzmax = (P2.z - origin.z) *divz;
            }
            else {
                tzmin = (P2.z - origin.z) *divz;
                tzmax = (P1.z - origin.z) *divz;
            }
            if ( (tmin > tzmax) || (tzmin > tmax) )
                return -1.0;

            if (tzmin > tmin)
                tmin = tzmin;
            if (tzmax < tmax)
                tmax = tzmax;


            a = tmin;
            b = tmax;
            return true;

            //return((tmin < ray.max) && (tmax > ray.min));
        }

};
#endif
