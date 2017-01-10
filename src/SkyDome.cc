#include "SkyDome.h"
#include <iostream>
#include <fstream>

SkyDome::SkyDome() : skyDomeType(SOLID), solidColor(Vector(0)), radianceValues(NULL), width(0), height(0)
{}

SkyDome::~SkyDome(void)
{
        delete[] radianceValues;
        return;
}

void SkyDome::setSolidColor(Vector color)
{
    skyDomeType = SOLID;
    solidColor = color.power(GAMMA);
    delete[] radianceValues;
    return;
}

void SkyDome::setImage(std::string fileName)
{
    skyDomeType = SKYDOME;
    delete[] radianceValues;

    std::ifstream myFile(fileName.c_str(), std::ios::in | std::ios::binary);
    if (myFile.is_open())
    {
        myFile.seekg (0, std::ios::end);
        unsigned int size = myFile.tellg();
        if(size<18)
        {
            std::cout<<"SkyDome::SkyDome(): File too small: " << fileName << " (" << size << " B)" << '\n';
            return;
        }

        // Let's read the dimensions of the image
        myFile.seekg(12, std::ios::beg);
        char dimension[4];
        myFile.read(dimension, 4);
        width = (unsigned char) dimension[0]|((unsigned char)dimension[1]<<8);
        height = (unsigned char) dimension[2]|((unsigned char) dimension[3]<<8);

        std::cout<<"SkyDome image: "<<fileName<<" ("<<width<<'x'<<height<<")\n";

        // Now read in the actual pixel values
        size -= 18;
        char *imageBuffer = new char [size];
        myFile.seekg(18, std::ios::beg);
        myFile.read(imageBuffer, size);

        size/=3;
        radianceValues = new Vector[size];
        for(unsigned int i=0; i<size;i++)
        {
            radianceValues[i].z=(unsigned char) imageBuffer[3*i]*(1.0/255.0);
            radianceValues[i].y=(unsigned char) imageBuffer[3*i+1]*(1.0/255.0);
            radianceValues[i].x=(unsigned char) imageBuffer[3*i+2]*(1.0/255.0);
            radianceValues[i]=radianceValues[i].power(GAMMA);
        }
        delete imageBuffer;
    }
   else 
       std::cout<<"SkyDome::SkyDome(): Unable to open file: " << fileName << '\n';
   return;
}

Vector SkyDome::getRadiance(Vector& direction) const
{
    switch(skyDomeType)
    {
        case SOLID:
            return solidColor;
            break;
        case SKYDOME:
#define LBOUND (0)
            if(direction.y < LBOUND)
                return Vector(0.0);

            myFloat alpha = acos(direction.y);
            myFloat beta = atan2(direction.x, direction.z)+M_PI;
            unsigned int y = (unsigned int) ((2+LBOUND)*height*alpha*(1.0/M_PI));
            unsigned int x = (unsigned int) (width*beta*(1.0/(2.0*M_PI)));

            return(radianceValues[y*width+x]);
            break;
    }
    std::cout<<"SkyDome::getRadiance(): Invalid skyDomeType.\n";
    abort();
    return Vector(0);
}
