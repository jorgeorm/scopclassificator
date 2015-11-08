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
    _scaled = false;
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

    _scaled = other.scaled();

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
T *Matrix<T>::row(unsigned i) const {
    return _data[i];
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
bool Matrix<T>::scaled() const{
    return _scaled;
}

template<typename T>
void Matrix<T>::setScaled(bool flag){
    _scaled = flag;
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
