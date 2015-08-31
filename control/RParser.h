#ifndef RPARSER_H
#define RPARSER_H

#include "Rcpp.h"
#include "RInside.h"
#include "modelo/matriz.h"
#include "control/Procesors/RServClient.h"
#include <QString>

using namespace Rcpp;
using namespace std;

class RParser
{
public:
    RParser();

    /**
     * @brief parsearMatrices toma un vector con Matrices y las transforma en una sola matriz de datos para R y CLARA
     * @param conjuntoMatrices <std::vector <Matriz *>> dataset de matrices a parsear en matriz numérica de R
     * @return char* nombre variable de matriz numérica de R para algoritmos de clustering
     */
    static char* parsearMatrices(double ** matriz, int tamMat, int tamSmat, RInside &instanciaR);


    /**
     * @brief parsearMatrizClienteR parsea una matriz al cliente de Rserve
     * Obtiene los features para la matriz que es pasada como parametro y almacena cada uno en el entorno del RServlet como una fila en el dataset con el cual se trabajara
     * @param matriz <double **> Puntero a matriz con datos, la cual será preprocesada y almacenada en el entorno de R
     * @param i_rows Número de filas de la matriz original
     * @param i_cols Número de columnas de la matriz original
     * @param i_rowsSub Número de filas en la submatriz a representar el feature
     * @param i_colsSub Número de columnas en la submatriz a representar el feature
     * @param rscl_entorno <RservletClient *> Puntero al cliente al cual se le cargaran los datos
     * @param nomVar <QString> nombre de la variable que contendra los datos almacenados en la instancia de RServletCliient
     */
    static void parsearMatrizClienteR ( double ** matriz, int i_rows, int i_cols, int i_rowsSub, int i_colsSub,  RServClient * rscl_entorno, QString nomVar);


private:
    /**
     * @brief fileDataset
     */
    static const std::string fileDataset;

    /**
     * @brief featureCollection
     */
    static const std::string fileFeatures;
};

#endif // RPARSER_H
