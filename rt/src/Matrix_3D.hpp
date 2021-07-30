#pragma once

#include "Vector_3D.hpp"

class Matrix_3D {
    protected:
    struct MatrixRow {
        const int size = 4;
        double row[4];
        double &operator[](const int i) { return row[i]; }
        const double &operator[](const int i) const { return row[i]; }
    };

    MatrixRow m[4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    public:

    Matrix_3D() {}
    MatrixRow &operator[](const int i) { return m[i]; }
    const MatrixRow &operator[](const int i) const { return m[i]; }

    void identity ();
    void matrix_cat (const Matrix_3D &m2);
    void trans_vector (const Vec &in , Vec &out);
    void trans_normal (Vec &in , Vec &out);
    void lubksb(int *indx, double b[]);
    void ludcmp(int *indx, double *d);
    void matrix_copy(const Matrix_3D &m1);
    Matrix_3D &matrix_inverse ();
};

typedef Matrix_3D Matrix;
