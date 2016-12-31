#ifndef RANDOM_H
#define RANDOM_H
#include "Vector.h"
class Random
{
    private:
        unsigned int MT[624];
        int MTIndex;
        
    public:

        Random(unsigned int seed = 5489UL) : MTIndex(624)
        {
            randomSeed(seed);
            return;
        }

        void randomSeed(unsigned int seed)
        {
            MT[0] = seed;
            for(int i = 1; i<624; i++)
                MT[i] = 0xffffffffUL & (1812433253UL * (MT[i-1] ^ (MT[i-1] >> 30)) + i);

            return;
        }

        // Return a random number in closed interval [0,1]
        myFloat randomNumberClosed(void)
        {
            static unsigned int andArray[2] = {0, 0x9908b0dfUL};
            if(MTIndex == 624)
            {
                for (int i = 0; i < 227; i++)
                {
                    unsigned int value = (MT[i] & 0x80000000UL) | (MT[i+1] & 0x7fffffffUL);
                    MT[i] = MT[i + 397] ^ (value >> 1) ^ (andArray[(value&1)]);
                }

                for (int i = 227; i < 623; i++) {
                    unsigned int value = (MT[i] & 0x80000000UL) | (MT[i+1] & 0x7fffffffUL);
                    MT[i] = MT[i - 227] ^ (value >> 1) ^ (andArray[(value&1)]);
                }

                unsigned int value = (MT[623] & 0x80000000UL) | (MT[0] & 0x7fffffffUL);
                MT[623] = MT[396] ^ (value >> 1) ^ (andArray[(value&1)]);
                MTIndex = 0;
            }
            unsigned int value = MT[MTIndex++];
            
            value ^= value >> 11;
            value ^= (value<<7) & (2636928640UL);
            value ^= (value<<15) & (4022730752UL);
            value ^= value>>18;

            return (myFloat) value * (1.0 / 4294967295.0);
        }

        // Return a random number in an open interval [0,1)
        myFloat randomNumberOpen(void)
        {
            static unsigned int andArray[2] = {0, 0x9908b0dfUL};
            if(MTIndex == 624)
            {
                for (int i = 0; i < 227; i++)
                {
                    unsigned int value = (MT[i] & 0x80000000UL) | (MT[i+1] & 0x7fffffffUL);
                    MT[i] = MT[i + 397] ^ (value >> 1) ^ (andArray[(value&1)]);
                }

                for (int i = 227; i < 623; i++) {
                    unsigned int value = (MT[i] & 0x80000000UL) | (MT[i+1] & 0x7fffffffUL);
                    MT[i] = MT[i - 227] ^ (value >> 1) ^ (andArray[(value&1)]);
                }

                unsigned int value = (MT[623] & 0x80000000UL) | (MT[0] & 0x7fffffffUL);
                MT[623] = MT[396] ^ (value >> 1) ^ (andArray[(value&1)]);
                MTIndex = 0;
            }
            unsigned int value = MT[MTIndex++];

            value ^= value >> 11;
            value ^= (value<<7) & (2636928640UL);
            value ^= (value<<15) & (4022730752UL);
            value ^= value>>18;

            return (myFloat) value * (1.0 / 4294967296.0);
        }

};
#endif
