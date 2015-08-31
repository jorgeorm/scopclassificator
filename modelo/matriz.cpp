#include "matriz.h"

void Matriz::initMatriz(int numRows, int numCols){
    this->numRows = numRows;
    this->numCols = numCols;

    if(numRows != numCols) {
        this->square = false;
    }else {
        this->square=true;
    }

    this->matriz = new double * [numRows];

    for(int idRow = 0; idRow< numRows; ++idRow){
        this->matriz[idRow] = new double [numCols];

        for(int idCol = 0; idCol < numCols; ++idCol){
            this->matriz[idRow][idCol] = 0;
        }
    }
}

Matriz::Matriz( int numRows, int numCols){
    initMatriz(numRows, numCols);
}

Matriz::Matriz(int numRowsCols){
    initMatriz(numRowsCols, numRowsCols);
}

Matriz::~Matriz(){
    for(int idRow = 0; idRow < numRows; idRow++){
        delete this->matriz[idRow];
    }
    delete this->matriz;

    this->matriz = 0;

    numRows = numCols = 0;

    square = false;
}

double Matriz::getElement(int x, int y){
    return matriz[x][y];
}

void Matriz::setElement(int x, int y, double value){
    this->matriz[x][y] = value;
}

bool Matriz::isSquare(){
    return this->square;
}

int Matriz::getNumCols(){
    return this->numCols;
}

int Matriz::getNumRows(){
    return this->numRows;
}
