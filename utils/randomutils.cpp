#include "randomutils.h"


#include <QString>
#include <ctime> // Para el time como seed
#include <fstream> // Lectura/Escritura archivos

RandomUtils::RandomUtils()
{
}

unsigned int RandomUtils::randomSeed(){
    unsigned int random_seed, random_seed_a, random_seed_b;
     std::ifstream file ("/dev/random", std::ios::binary);
     if (file.is_open())
     {
         char * memblock;
         int size = sizeof(int);
         memblock = new char [size];
         file.read (memblock, size);
         file.close();

         QString memQstr(memblock);

         random_seed_a = memQstr.toInt();
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


