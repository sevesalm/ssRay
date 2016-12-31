#ifndef VECTOR_H
#define VECTOR_H
#include "utils.h"
#include <sstream>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <limits>
typedef float myFloat;

//#define EPSILON 0.0000001
#define EPSILON 0.0001
#define MAX_FLOAT std::numeric_limits<myFloat>::max()
#define MIN_FLOAT -std::numeric_limits<myFloat>::max()
#define MAX_UINT std::numeric_limits<unsigned int>::max()
#define GAMMA 2.2
#define PASSES 100 
class Vector
{
    private:
    public:
        myFloat x;
        myFloat y;
        myFloat z;

        Vector() : x(0.0), y(0.0), z(0.0)
        {
        }

        Vector(myFloat x, myFloat y, myFloat z) : x(x), y(y), z(z)
        {
        }
        
        Vector(myFloat c) : x(c), y(c), z(c)
        {
        }

        /*explicit Vector(const Point& point) : x(point.x), y(point.y), z(point.z)
        {
        }*/

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
            std::cout<<"Vector::operator[]: Wrong axis!\n";
            abort();
        }

        Vector operator+(const Vector& vector) const
        { 
            return Vector(x+vector.x, y+vector.y, z+vector.z);
        }

        Vector operator-() const
        {
            return Vector(-x, -y, -z);
        }

        Vector operator-(const Vector& vector) const
        {
            return Vector(x-vector.x, y-vector.y, z-vector.z);
        }
        
        Vector operator*(myFloat scale) const
        {
            return Vector(x*scale, y*scale, z*scale);
        }
        Vector operator/(myFloat divider) const
        {
            myFloat inverse = 1.0 / divider;
            return Vector(x*inverse, y*inverse, z*inverse);
        }

        // Dot product a1b1 + a2b2 + a3c3

        myFloat dot(const Vector& vector) const
        {
            return vector.x*x + vector.y*y + vector.z*z;
        }

        friend const Vector operator*(const myFloat value, const Vector& vector)
        {
            return Vector(vector.x*value, vector.y*value, vector.z*value);
        }

        // Cross product (a2b3 − a3b2, a3b1 − a1b3, a1b2 − a2b1)

        Vector cross(const Vector& vector) const
        {
            return Vector(y*vector.z - z*vector.y, z*vector.x - x*vector.z, x*vector.y - y*vector.x);
        }

        
        Vector& operator=(const Vector& vector)
        {
            x = vector.x;
            y = vector.y;
            z = vector.z;

            return *this;
        }

        Vector& operator+=(const Vector& vector)
        {
            *this = *this + vector;
            return *this;
        }

        Vector& operator-=(const Vector& vector)
        {
            *this = *this - vector;
            return *this;
        }

        Vector& operator*=(const myFloat divider)
        {
            *this = *this * divider;
            return *this;
        }

        Vector& operator/=(const myFloat divider)
        {
            *this = *this / divider;
            return *this;
        }

        bool operator==(const Vector& vector) const
        {
            return (x==vector.x && y==vector.y && z==vector.z);
        }

        bool operator!=(const Vector& vector) const
        {
            return(x!=vector.x || y!=vector.y || z!=vector.z);
        }

        Vector combine(const Vector& vector) const
        {
            return Vector(x*vector.x, y*vector.y, z*vector.z);
        }

        Vector perpendicular() const
        {
            myFloat dm = std::min(std::min(fabs(x),fabs(y)),fabs(z));
            if(dm == fabs(z))
                return (*this).cross(Vector(0,0,1)).unit();
            if(dm == fabs(y))
                return (*this).cross(Vector(0,1,0)).unit();
            
            return (*this).cross(Vector(1,0,0)).unit();
        }

        /*
         * Orient the vector with the provided bias vector so that if bias is (0,1,0) the 
         * original vector remains unchanged.
         */
        
        Vector orientY(const Vector& bias) const
        {
            if(x==0 && z==0)
            {
                if(fabs(y-1.0) < EPSILON)
                    return bias;    
                else if(fabs(y+1) < EPSILON)
                    return -bias;
            }
                
            // Optimized cross product because temp is (0,1,0)
            myFloat invLength = 1.0/sqrt(z*z+x*x);
            Vector tangent = Vector(-z*invLength, 0, x*invLength);
            Vector sTangent = Vector(y*tangent.z, z*tangent.x-x*tangent.z, -y*tangent.x);
            return bias.x*sTangent + bias.y*(*this) + bias.z*tangent;
        }
            
        myFloat length() const
        {
            return sqrt(x*x + y*y + z*z);
        }

        myFloat length2() const
        {
            return x*x+y*y+z*z;
        }

        Vector unit() const
        {
            
            return Vector(*this/this->length());
            //return Vector(x/length(), y/length(), z/length());
        }

        Vector power(myFloat exponent) const
        {
            return Vector(pow(x, exponent), pow(y, exponent), pow(z, exponent));
        }

        std::string toString() const
        {
            std::ostringstream infoString;

            infoString << '(' << x << ',' << y << ',' << z << ')';
            return infoString.str();
        }
};



#endif
