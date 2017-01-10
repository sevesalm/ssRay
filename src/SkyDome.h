#ifndef SKYDOME_H
#define SKYDOME_H
#include <string>
#include "Vector.h"

class SkyDome
{
    private:
        enum {SOLID, SKYDOME} skyDomeType;
        Vector solidColor;
        Vector *radianceValues;
        unsigned int width;
        unsigned int height;
    public:
        SkyDome();
        ~SkyDome();
        void setImage(std::string);
        void setSolidColor(Vector);
        Vector getRadiance(Vector &) const;
};

#endif
