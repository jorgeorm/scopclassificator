#include "arrayutils.h"

ArrayUtils::ArrayUtils()
{
}

/**
 * @brief dp_initArray
 * @param tamano
 * @return pdarr_arreglo
 */
double * ArrayUtils::dp_initArray( int tamano ){
    double * pdarr_arreglo = new double [ tamano ];
    for(int id = 0; id < tamano; ++id){
        pdarr_arreglo [id] = 0;
    }

    return pdarr_arreglo;
}

/**
 * @brief dp_initMatriz
 * @param numRows
 * @param numCols
 * @return pdmat_matriz
 */
double ** ArrayUtils::dp_initMatriz( int numRows, int numCols ){

    double ** pdmat_matriz = new double * [numRows];

    for( int idRow = 0; idRow < numRows; ++idRow ){
        pdmat_matriz [ idRow ] = new double [numCols];
        for( int idCol = 0; idCol < numCols; ++idCol ){
            pdmat_matriz [ idRow ][ idCol ] = 0;
        }
    }

    return pdmat_matriz;

}

/**
 * @brief deleteMatriz
 * @param pdmat_matriz
 * @param numRows
 */
void ArrayUtils::deleteMatriz( double **  pdmat_matriz, int numRows ){

    for ( int idRow = 0; idRow < numRows; idRow++ ){
        delete [] pdmat_matriz [ idRow ];
    }

    delete [] pdmat_matriz;

}
