#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <random>
#include <iostream>



class RandomGenerator
{
    typedef std::mt19937 MyRNG;  // the Mersenne Twister with a popular choice of parameters

public:
    RandomGenerator();

    static unsigned int generateSeed();
    int randomIntBetween(int low, int sup);

private:

    MyRNG _rng; // e.g. keep one global instance (per thread)
    unsigned int _seed;
    std::uniform_int_distribution<int> _uint_dist;
};

#endif // RANDOMGENERATOR_H
