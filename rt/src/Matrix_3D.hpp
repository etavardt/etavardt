#pragma once
#include "Vector_3D.hpp"

//typedef double Matrix[4][4];

class Matrix_3D {
    protected:
    struct MatrixRow {
        const int size = 4;
        double row[4];
        double &operator[](const int i) { return row[i]; }
        const double &operator[](const int i) const { return row[i]; }
    };
//    Matrix m = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    MatrixRow m[4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    public:

    Matrix_3D() {}
    MatrixRow &operator[](const int i) { return m[i]; }
    const MatrixRow &operator[](const int i) const { return m[i]; }

// TODO: TCE: Finish Matrix class and make these functions non-static and
//            Fix callers parser and shade to use the transform matirix to
//            call these some may be moved there.
    static void identity (Matrix_3D &mat);
    static void matrix_cat (Matrix_3D &m1 , Matrix_3D &m2 , Matrix_3D &dest);
    static void trans_vector (Matrix_3D &mat , Vec &in , Vec &out);
    static void trans_normal (Matrix_3D &mat , Vec &in , Vec &out);
    static void lubksb(Matrix_3D &a, int *indx, double b[]);
    static void ludcmp(Matrix_3D &a, int *indx, double *d);
    static void matrix_copy(Matrix_3D &m1, Matrix_3D &m2);
    static void matrix_inverse (Matrix_3D &in , Matrix_3D &out);

};

typedef Matrix_3D Matrix;
