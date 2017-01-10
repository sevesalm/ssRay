#include "Film.h"
#include <iostream>
#include <fstream>

Film::Film(unsigned int width, unsigned int height) : width(width), height(height), counter(0)
{
    radianceValues = new Vector[width*height];
    radianceSamples = new unsigned int[width*height];
    for(unsigned int i=0; i<width*height; i++)
    {
        radianceValues[i] = Vector(0);
        radianceSamples[i] = 0;
    }
    return;
}

Film::~Film(void)
{
    delete[] radianceValues;
    delete[] radianceSamples;
    return;
}

Film& Film::operator=(const Film& film)
{
    if(width != film.width || height != film.height)
    {
        std::cout<<"Film::operator=: different film sizes!\n";
        abort();
    }
    for(unsigned int i = 0; i < width*height; i++)
    {
       radianceValues[i] = film.radianceValues[i];
       radianceSamples[i] = film.radianceSamples[i];
    }
    return *this;
}

Film& Film::operator+=(const Film& film)
{

    if(width != film.width || height != film.height)
    {
        std::cout<<"Film::operator+=: different film sizes!\n";
        abort();
    }

    for(unsigned int i = 0; i < width*height; i++)
    {
       radianceValues[i] += film.radianceValues[i];
       radianceSamples[i] += film.radianceSamples[i];

    }

    return *this;
}

void Film::clear(void)
{
    for(unsigned int i = 0; i<width*height;i++)
        radianceSamples[i] = 0;
    return;
}

void Film::writeRadiance(unsigned int x, unsigned int y, Vector radiance)
{
    unsigned int index = y*width + x;

    if(index >= width*height)
    {
        std::cout<<"Film::writeRadiance(): x/y out of range!\n";
        abort();
    }

    myFloat samples = ++radianceSamples[index];
    radianceValues[index] = (samples-1)/samples * radianceValues[index] + radiance/samples;

    return;
}

Vector Film::readRadiance(unsigned int x, unsigned int y) const
{
    unsigned int index = y*width + x;

    if(index >= width*height)
    {
        std::cout<<"Film::readRadiance(): x/y out of range!\n";
        abort();
    }
    
    return radianceValues[index];
}

myFloat Film::readBrightness(unsigned int x, unsigned int y) const
{
    unsigned int index = y*width + x;

    if(index >= width*height)
    {
        std::cout<<"Film::readRadiance(): x/y out of range!\n";
        abort();
    }
    Vector radiance = radianceValues[index];
    return (radiance.x + radiance.y + radiance.z) / 3.0;
}

char Film::pixelValue(myFloat value) const
{
    myFloat gammaCorrectedValue = value;
    if(GAMMA!=1.0)
//        myFloat inverseGamma = 1.0/GAMMA;
        gammaCorrectedValue = pow(value, 1.0/GAMMA);
    
    
    if(gammaCorrectedValue >= 1.0)
        return 0xff;

    return (char) (0xff * gammaCorrectedValue);
}

unsigned int Film::getSamples(unsigned int x, unsigned int y) const
{
    unsigned int index = y*width + x;
    if(index >= width*height)
    {
        std::cout<<"Film::getSamples(): x/y out of range!\n";
        abort();
    }

    return radianceSamples[index];
}


unsigned int Film::getMinSamples(RectangleInfo *area) const
{
    unsigned int minimum = MAX_UINT;
    for(unsigned int y = area->ystart; y <= area->yend; y++)
        for(unsigned int x = area->xstart; x <= area->xend; x++)
            if(radianceSamples[y*width+x] < minimum)
                minimum = radianceSamples[y*width+x];
    
    return minimum;
}

unsigned int Film::getMaxSamples(RectangleInfo *area) const
{
    unsigned int maximum = 0;
    for(unsigned int y = area->ystart; y <= area->yend; y++)
        for(unsigned int x = area->xstart; x <= area->xend; x++)
            if(radianceSamples[y*width+x] > maximum)
                maximum = radianceSamples[y*width+x];

    return maximum;
}

void Film::save(std::string fileName, char *pixelData)
{
    std::ofstream myfile(fileName.c_str(), std::ios::out | std::ios::binary);

    if (myfile.is_open())
    {
        //This is the TGA file header - 18bytes. 32bit data, origin at top-left. 
        char header[] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, (unsigned char) (width&0xff), (unsigned char) ((width&0xff00)>>8), (unsigned char) (height&0xff), (unsigned char) ((height&0xff00)>>8), 24, 1<<5};

        myfile.write(header, 18);
        myfile.write(pixelData, width * height * 3);
    }
    else std::cout<<"Film::save(): Unable to open file: " << fileName << '\n';

    return;
}

unsigned int Film::getWidth(void) const
{
    return width;
}

unsigned int Film::getHeight(void) const
{
    return height;
}

