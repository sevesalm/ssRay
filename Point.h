#ifndef POINT_H
#define POINT_H
#include "Vector.h"
#include <sstream>
#include <cmath>
class Point
{
    private:

    public:
        myFloat x;
        myFloat y;
        myFloat z;

        Point(void) : x(0.0), y(0.0), z(0.0)
        {
        }

        Point(myFloat x, myFloat y, myFloat z) : x(x), y(y), z(z)
        {
        }
        
        explicit Point(const Vector& vector) : x(vector.x), y(vector.y), z(vector.z)
        {
        }

        myFloat& operator[](const unsigned int i)
        {
            switch(i)
            {
                case 0:
                    return x;
                    break;
                case 1:
                    return y;
                    break;
                case 2:
                    return z;
                    break;
            }
            std::cout<<"Point::operator[]: Wrong axis!\n";
            abort();
        }

        Point operator+(const Vector& vector) const
        { 
            return Point(x+vector.x, y+vector.y, z+vector.z);
        }

        Point operator-(const Vector& vector) const
        {
            return Point(x-vector.x, y-vector.y, z-vector.z);
        }

        Vector operator-(const Point& point) const
        {
            return Vector(x-point.x, y-point.y, z-point.z);
        }

        Point operator/(myFloat divider) const
        {
            myFloat inverse = 1.0 / divider;
            return Point(x*inverse, y*inverse, z*inverse);
        }

        Point operator*(myFloat scale) const
        {
            return Point(x*scale, y*scale, z*scale);
        }

        friend const Point operator*(const myFloat scale, const Point& point)
        {
            return Point(point.x*scale, point.y*scale, point.z*scale);
        }


        Point& operator=(const Point& point)
        {
            x = point.x;
            y = point.y;
            z = point.z;

            return *this;
        }


        Point& operator+=(const Vector& vector)
        {
            *this = *this + vector;
            return *this;
        }

        Point& operator-=(const Vector& vector)
        {
            *this = *this - vector;
            return *this;
        }

        Point& operator*=(const myFloat divider)
        {
            *this = *this * divider;
            return *this;
        }

        Point& operator/=(const myFloat divider)
        {
            *this = *this / divider;
            return *this;
        }

        bool operator==(const Point& point) const
        {
            return (x==point.x && y==point.y && z==point.z);
        }

        bool operator!=(const Point& point) const
        {
            return(x!=point.x || y!=point.y || z!=point.z);
        }
        std::string toString(void) const
        {
            std::ostringstream infoString;

            infoString << '(' << x << ',' << y << ',' << z << ')';
            return infoString.str();
        }
};
#endif
