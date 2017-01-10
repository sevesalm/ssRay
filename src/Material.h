#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vector.h"
#include "Point.h"
#include <iostream>
#include <vector>

class Material
{
    private:
        std::vector<Vector> colors;
        enum {SOLID, CHECKERS, GRID} pigmentType;
        myFloat pigmentScale;
        bool emitting;
        myFloat specularCoef;
        myFloat glossiness;
        myFloat refractiveCoef;
        myFloat refractiveIndex;

    public:
       
        Material() : pigmentScale(1.0), emitting(false), specularCoef(0.0), glossiness(1.0), refractiveCoef(0.0), refractiveIndex(1.0)
        {
            colors.push_back(Vector(1.0));
            pigmentType = SOLID;
        }
        
        Vector getColor(Point& p) const
        {
            Point pigmentPoint = p*pigmentScale;
            switch(pigmentType)
            {
                unsigned int i;

                case SOLID:
                    return colors[0];

                case CHECKERS:
                    i = ((unsigned int) floor(pigmentPoint.x) % 2) ^ (((unsigned int) floor(pigmentPoint.y)) % 2) ^ (((unsigned int) floor(pigmentPoint.z)) % 2);
                    return colors[i];
                case GRID:
                    i = ((pigmentPoint.x-floor(pigmentPoint.x)) > 0.05) && ((pigmentPoint.z-floor(pigmentPoint.z)) > 0.05);
                    return colors[i];
            }

            std::cout<<"Object::getColor(): Invalid pigmentType: " << pigmentType << '\n';
            abort();
            return Vector(0);
        }
        
        void setPigmentSolid(Vector color)
        {
            pigmentType = SOLID;
            colors.clear();
            colors.push_back(color.power(GAMMA));
            return;
        }

        void setPigmentCheckers(Vector color1, Vector color2)
        {
            pigmentType = CHECKERS;
            colors.clear();
            colors.push_back(color1.power(GAMMA));
            colors.push_back(color2.power(GAMMA));
            return;
        }

        void setPigmentGrid(Vector color1, Vector color2)
        {
            pigmentType = GRID;
            colors.clear();
            colors.push_back(color1.power(GAMMA));
            colors.push_back(color2.power(GAMMA));
            return;
        }
        void setPigmentScale(myFloat scale)
        {
            pigmentScale = scale;
            return;
        }

        void setEmitting(bool emission)
        {
            emitting = emission;
            return;
        }

        void setRefractiveIndex(myFloat index)
        {
            refractiveIndex = index;
            return;
        }

        void setSpecularCoef(myFloat coef)
        {
            specularCoef = coef;
            return;
        }

        void setRefractiveCoef(myFloat coef)
        {
            refractiveCoef = coef;
            return;
        }

        void setGlossiness(myFloat roughness)
        {
            glossiness = roughness;
            return;
        }

        bool isEmitting(void) const
        {
            return emitting;
        }

        myFloat getRefractiveIndex(void) const
        {
            return refractiveIndex;
        }

        myFloat getSpecularCoef(void) const
        {
            return specularCoef;
        }
        
        myFloat getGlossiness(void) const
        {
            return glossiness;
        }
        myFloat getRefractiveCoef(void) const
        {
            return refractiveCoef;
        }
};

#endif
