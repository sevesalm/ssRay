#include "utils.h"
#include "Vector.h"
#include <ctime>
#include <iostream>
#include <sstream>
#include <iomanip>


double getTime(void)
{
    timespec tp;
    clock_gettime(CLOCK_REALTIME, &tp);
    return (tp.tv_sec +  tp.tv_nsec/1000000000.0);
}

std::string timeToString(double timeInSeconds)
{
    std::ostringstream timeString;

    unsigned int totalSeconds = (unsigned int) timeInSeconds;
    unsigned int seconds=0;
    unsigned int minutes=0;
    unsigned int hours=0;
    unsigned int days=0;

    days = totalSeconds/(60*60*24);
    totalSeconds -= days*60*60*24;
    hours = totalSeconds/(60*60);
    totalSeconds -= hours*60*60;
    minutes = totalSeconds/60;
    totalSeconds -= minutes*60;
    seconds = totalSeconds;

    if(days)
        timeString << days << "d ";
    if(hours)
        timeString << hours << "h ";
    if(minutes)
        timeString << minutes << "m "; 

    timeString << std::setiosflags(std::ios_base::fixed) << std::setprecision(1) << seconds << 's';

    return timeString.str();
}


/*
myFloat halton(myFloat value, int base)
{
    myFloat r = 1.0 - value - EPSILON;
    myFloat invBase = 1.0 / base;
    if (invBase < r)
        value += invBase;
    else 
    {
        myFloat hh, h=invBase;
        do 
        {
            hh = h;
            h *= invBase;
        } 
        while (h >= r);
        value += hh + h - 1.0;
    }
    return value;
}

myFloat halton(int index, int base)
{
    myFloat output = 0.0;
    myFloat fraction = 1.0;
    while(index)
    {
        fraction /= base;
        output += fraction * (index%base);
        index /= base;
    }
    return output;
}
*/
