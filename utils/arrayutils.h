#ifndef ARRAYUTILS_H
#define ARRAYUTILS_H

class ArrayUtils
{
public:

    static double * dp_initArray( int tamano );

    static double ** dp_initMatriz( int numRows, int numCols );

    static void deleteMatriz( double **  pdmat_matriz, int numRows );

private:
    ArrayUtils();

};

#endif // ARRAYUTILS_H
