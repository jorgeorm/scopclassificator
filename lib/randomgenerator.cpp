#include "randomgenerator.h"
#include <random>

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <climits>


RandomGenerator::RandomGenerator()
{
    _seed = generateSeed();
    _rng.seed(_seed);
}

int RandomGenerator::randomIntBetween(int low, int high){


    int bits, val;
    int bound = high - low;

    do {
        bits = _uint_dist(_rng);
        val = bits % bound;
    } while (bits - val + (bound) < 0);

    val += low;

    return val;
}

unsigned RandomGenerator::generateSeed(){

    unsigned random_seed, random_seed_a, random_seed_b;
    std::ifstream file ("/dev/random", std::ios::binary);
    if (file.is_open())
    {
        unsigned char * memblock;
        int size = sizeof(unsigned);
        memblock = new unsigned char [size];
        file.read ((char *)memblock, size);
        file.close();
        random_seed_a = (unsigned char)memblock[0] << CHAR_BIT;
        for (int ibyte = 1; ibyte < size; ++ibyte) {
            random_seed_a |= (unsigned char) memblock[ibyte];
        }
        delete[] memblock;
    }// end if
    else
    {
        random_seed_a = 0;
    }
    random_seed_b = std::time(0);
    random_seed = random_seed_a xor random_seed_b;
    return random_seed;

}
