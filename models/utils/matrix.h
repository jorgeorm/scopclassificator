#ifndef SCMATRIX_H
#define SCMATRIX_H
#include <QDebug>
#include <vector>
#include "Rcpp.h"

template <typename T>
class Matrix {
public:
    Matrix(unsigned rows, unsigned cols, const T &initial = 0);
    Matrix(const Matrix<T> &other);
    virtual ~Matrix();

    unsigned rows() const;

    unsigned cols() const;

    T* row(unsigned i) const;

    T valueAt(unsigned i, unsigned j) const;

//    std::vector<std::vector<T>> data() const;

//---------------OPERATORS---------------
    Matrix<T> &operator=(const Matrix<T> &other);

    T& operator()(const unsigned& row, const unsigned& col);

    // Access the individual elements (const)
    const T& operator()(const unsigned& row, const unsigned& col) const;

    /**
     * @brief toRcppMatrix
     * @return
     */
    Rcpp::NumericMatrix toRcppMatrix();


    /**
     * @brief maxVal
     * @return
     */
    T maxVal() const;

    /**
     * @brief setValue
     * @param i
     * @param j
     * @param value
     */
    void setValue(unsigned i, unsigned j, T value);

    /**
     * @brief scaled Indicates if the matrix has been scaled
     * @return
     */
    bool scaled() const;
    void setScaled(bool flag);

signals:
    void dataChanged();

private:
    template <typename TN>
    friend std::ostream& operator<<(std::ostream &os, const Matrix<TN> &matrix);
    template <typename TN>
    friend QDebug operator<<(QDebug dbg, const Matrix<TN> &matrix);

    unsigned _rows;
    unsigned _cols;
    bool _scaled;
    T _maxVal;
    T** _data;
};

template<typename T>
QDebug operator<<(QDebug dbg, const Matrix<T> &matrix) {
    dbg.nospace()<<"Rows: "<< matrix.rows()<<", ";
    dbg.nospace()<<"Columns: "<< matrix.cols()<<"\n";

    for(unsigned i = 0; i < matrix.rows(); ++i){
        for (unsigned j = 0; j < matrix.cols(); ++j){
            dbg.nospace()<< matrix.valueAt(i, j);
            if (j < matrix.cols()-1) dbg.nospace()<<", ";
        }
        if (i < matrix.rows()-1) dbg.nospace()<<"\n";
    }

    return dbg.maybeSpace();
}
template<typename T>
std::ostream& operator<<(std::ostream &os, const Matrix<T> &matrix) {
    os<<"Rows: "<< matrix.rows()<<", ";
    os<<"Columns: "<< matrix.cols()<<"\n";

    for(unsigned i = 0; i < matrix.rows(); ++i){
        for (unsigned j = 0; j < matrix.cols(); ++j){
            os<< matrix.valueAt(i, j);
            if (j < matrix.cols()-1) os<<", ";
        }
        if (i < matrix.rows()-1) os<<"\n";
    }

    return os;
}

#endif // SCMATRIX_H
