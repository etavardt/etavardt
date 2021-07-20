#pragma once
#include "Vector_3D.hpp"

typedef double Matrix[4][4];

class Matrix_3D {
    public:
    double m[4][4];

// TODO: TCE: Finish Matrix class and make these functions non-static and
//            Fix callers parser and shade to use the transform matirix to
//            call these some may be moved there.
    static void identity (Matrix mat);
    static void matrix_cat (Matrix m1 , Matrix m2 , Matrix dest);
    static void trans_vector (Matrix mat , Vec &in , Vec &out);
    static void trans_normal (Matrix mat , Vec &in , Vec &out);
    static void lubksb(Matrix a, int *indx, double b[]);
    static void ludcmp(Matrix a, int *indx, double *d);
    static void matrix_copy(Matrix m1, Matrix m2);
    static void matrix_inverse (Matrix in , Matrix out);

};
