#ifndef MATRIZ_H
#define MATRIZ_H

class Matriz
{
public:
    /**
     * \brief Objeto para representar una matriz, se creo con el fin de poder crear las submatrices
     * Constructor para una matriz que no sea cuadrada.
     * \param numRows <int> Numero de Filas
     * \param numCols <int> Numero de Columnas que tendra la matriz
     */
    Matriz(int numRows, int numCols);
    /**
     * \brief Objeto para representar una matriz, se creo con el fin de poder crear las submatrices
     * Constructor para una matriz cuadrada
     * \param numRowsCols <int> Numero de Filas
     */
    Matriz(int numRowsCols = 0);
    /**
     * \brief Destructor sin parametros
     */
    ~Matriz();

    /**
     * \brief Consulta lo que hay en la matriz en la posicion especificada
     * \param x <int> posicion en filas
     * \param y <int> posicion en columnas
     * \returns <double> elemento almacenado en la matriz
     */
    double getElement (int x, int y); /// < Getter para la posicion
    /**
     * \brief Setea la matriz en la posicion especificada
     * \param x <int> posicion en filas
     * \param y <int> posicion en columnas
     * \param value <double> valro a ser almacenado en la matriz
     */
    void setElement (int x, int y, double value); /// < Setter para la posicion

    int getNumRows(); /// < Permite obtener el numero de filas.

    int getNumCols(); /// < Permite obtener el numero de columnas.

    /**
     * \brief Consulta si la matriz es o no cuadrada
     * \return <bool> Si la matriz es cuadrada retorna TRUE, sino FALSE
    */
    bool isSquare();

private:
    int numRows, numCols;
    bool square;
    double ** matriz; /// < Matriz de dos dimensiones que puede ser accesada

    void initMatriz (int numRows, int numCols); /// < Funcion para inicializar la clase, permite reutilizar codigo

};

#endif // MATRIZ_H
