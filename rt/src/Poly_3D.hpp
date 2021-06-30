#pragma once
#include "Object_3D.hpp"

class Poly_3D: public Object_3D {
    public:
    Poly_3D();
    ~Poly_3D();

    int intersect(Object_3D *obj, Ray *ray, Isect &hit);
    void normal(Object_3D *obj, Isect &hit, Point P, Point N);

    static Poly_3D *makePoly(int npoints, Vec *points);
};