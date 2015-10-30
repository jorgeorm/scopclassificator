#include "matrix.h"

#include <QDebug>

template<typename T>
Matrix<T>::Matrix(unsigned rows, unsigned cols, const T &initial){
    _data = new T*[rows];
    for (int iRow = 0; iRow < rows; ++iRow) {
        _data[iRow] = new T[cols];
        for (int iCol = 0; iCol < cols; ++iCol){

            _data[iRow][iCol] = initial;
        }
    }


    _rows = rows;
    _cols = cols;
    _maxVal = initial;
}
template<typename T>
Matrix<T>::Matrix(const Matrix<T> &other) {
    _rows = other.rows();
    _cols = other.cols();
    _maxVal = other.maxVal();

    _data = new T*[_rows];

    for (int iRow = 0; iRow < _rows; ++iRow) {

        _data[iRow] = new T[_cols];
        for (int iCol = 0; iCol < _cols; ++iCol){

            _data[iRow][iCol] = other(iRow, iCol);
        }
    }


}

template <typename T>
Matrix<T>::~Matrix() {
    for (int i = 0; i < _rows; ++i)
        delete [] _data[i];
    delete [] _data;
}

template<typename T>
unsigned Matrix<T>::rows() const {
    return _rows;
}

template<typename T>
unsigned Matrix<T>::cols() const {
    return _cols;
}

template<typename T>
Matrix<T> &Matrix<T>::operator=(const Matrix<T> &other){
    if (&other != this) return *this;

    unsigned newRows = other.rows();
    unsigned newCols = other.cols();

    _data = new T*[newRows];
    for (unsigned i = 0; i < newRows; ++i) {
        _data[i] = new T[newCols];
    }

    T d;

    for (int i = 0; i < newRows; ++i) {
        for (int j = 0; j < newCols; ++j) {
            d = other.valueAt(i, j);
            _data[i][j] = d;
        }
    }

    _rows = newRows;
    _cols = newCols;
    _maxVal = other.maxVal();

    return *this;
}

template<typename T>
const T &Matrix<T>::operator()(const unsigned &row, const unsigned &col) const{
    return this->_data[row][col];
}

template<typename T>
Rcpp::NumericMatrix Matrix<T>::toRcppMatrix()
{
    Rcpp::NumericMatrix M(_rows,_cols);

    for (unsigned i=0; i < _rows; i++) {
        for (unsigned j=0; j < _cols; j++) {
            M(i,j) = _data[i][j];
        }
    }
    return(M);
}

template<typename T>
T Matrix<T>::maxVal() const
{
    return _maxVal;
}

template<typename T>
void Matrix<T>::setValue(unsigned i, unsigned j, T value){
    _data[i][j] = value;

    if (value > _maxVal) _maxVal = value;
}

template<typename T>
T &Matrix<T>::operator()(const unsigned &row, const unsigned &col){
    return this->_data[row][col];
}

template<typename T>
T Matrix<T>::valueAt(unsigned i, unsigned j) const{
    return _data[i][j];
}

template class Matrix<float>;
template class Matrix<int>;
