#ifndef FILM_H
#define FILM_H
#include "Vector.h"
#include "RectangleInfo.h"

class Film
{
    private:
        unsigned int width;
        unsigned int height;
        unsigned int counter;

        Vector *radianceValues;
        unsigned int *radianceSamples;

    public:
        Film(unsigned int, unsigned int);
        ~Film();

        Film& operator=(const Film&);
        Film& operator+=(const Film&);

        char pixelValue(myFloat) const;

        void writeRadiance(unsigned int, unsigned int, Vector);
        Vector readRadiance(unsigned int, unsigned int) const;
        myFloat readBrightness(unsigned int, unsigned int) const;
        unsigned int getMinSamples(RectangleInfo *) const; 
        unsigned int getMaxSamples(RectangleInfo *) const; 
        unsigned int getSamples(unsigned int, unsigned int) const;
        void clear();
        void save(std::string, char *);
        unsigned int getWidth() const;
        unsigned int getHeight() const;
};
#endif
