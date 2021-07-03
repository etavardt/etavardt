#pragma once
#include "Object_3D.hpp"

class Ring_3D: public Object_3D {
    public:
    Ring_3D();
    ~Ring_3D();

    int intersect(Object_3D *obj, Ray *ray, Isect &hit);
    void normal(Object_3D *obj, Isect &hit, Point P, Point N);

    static Ring_3D *makeRing(Vec pos, Vec norm, Flt min_rad, Flt max_rad);
};