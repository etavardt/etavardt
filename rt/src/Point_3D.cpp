#include "Point_3D.hpp"

#include "Vector_3D.hpp"

Point_3D &Point_3D::operator=(const Vector_3D &c) {
    v[0] = c[0];
    v[1] = c[1];
    v[2] = c[2];
    return *this;
}
