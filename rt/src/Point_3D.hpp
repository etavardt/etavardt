#pragma once
#include "Vector_3D.hpp"
class Point_3D : public Vector_3D {
    public:
    double &x = v[0];
    double &y = v[1];
    double &z = v[2];
    Point_3D() : Vector_3D() {}
    Point_3D(const Point_3D &p) : Vector_3D(p) {}
    Point_3D(const Vector_3D &p) : Vector_3D(p) {}
    Point_3D(const double a, const double b, const double c) : Vector_3D(a,b,c) {}
    ~Point_3D() {}
    Point_3D &operator=(const Point_3D &c);
    Point_3D &operator=(const Vector_3D &c);
};

typedef Point_3D Point;

// class Point_3D {
//     public:
//     double x;
//     double y;
//     double z;

//     Point_3D() : x(0), y(0), z(0) {}
//     Point_3D(const Point_3D &p) : x(p.x), y(p.y), z(p.z) {}
//     Point_3D(const Vector_3D &p) : x(p.v[0]), y(p.v[1]), z(p.v[2]) {}
//     Point_3D(const double a, const double b, const double c) : x(a), y(b), z(c) {}
// };

// typedef Vector_3D Point;
