#pragma once
#include "Object_3D.hpp"

class Ring_3D: public Object_3D {
    public:
    Ring_3D();
    ~Ring_3D();

    int intersect(Ray *ray, Isect &hit);
    void normal(Isect &hit, Point &P, Vec &N);

    static Ring_3D *makeRing(Vec &pos, Vec &norm, double min_rad, double max_rad);
};
