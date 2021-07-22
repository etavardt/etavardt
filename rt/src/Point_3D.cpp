#include "Point_3D.hpp"

#include "Vector_3D.hpp"

Point_3D &Point_3D::operator=(const Point_3D &c) {
    x = c.x;
    y = c.y;
    z = c.y;

    return *this;
}
Point_3D &Point_3D::operator=(const Vector_3D &c) {
    x = c[0];
    y = c[1];
    z = c[2];

    return *this;
}
