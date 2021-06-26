#pragma once
#include "Object_3D.hpp"

class Cone_3D: public Object_3D {
    public:
    Cone_3D();
    ~Cone_3D();

    int intersect(Object_3D *obj, Ray *ray, Isect *hit);
    void normal(Object_3D *obj, Isect *hit, Point P, Point N);

    static Cone_3D *makeCone(Vec basepoint, Flt baseradius, Vec apexpoint, Flt apexradius);
};