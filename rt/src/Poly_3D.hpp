#pragma once
#include "Object_3D.hpp"

class Poly_3D: public Object_3D {
    public:
    Poly_3D();
    ~Poly_3D();

    int intersect(Ray *ray, Isect &hit);
    void normal(Isect &hit, Point &P, Vec &N);

    static Poly_3D *makePoly(int npoints, Vec *points);
};
