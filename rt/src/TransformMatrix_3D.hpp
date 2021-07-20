#pragma once

#include "Matrix_3D.hpp"

class TransformMatrix_3D {
  public:
    Matrix mat;
    TransformMatrix_3D *next;
    static TransformMatrix_3D *transTop; /* top of transformation stack */
};
typedef TransformMatrix_3D Transform;
