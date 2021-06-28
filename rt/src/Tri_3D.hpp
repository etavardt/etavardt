#pragma once
#include "Object_3D.hpp"

class Tri_3D: public Object_3D {
    public:
    Tri_3D();
    ~Tri_3D();

    int intersect(Object_3D *obj, Ray *ray, Isect *hit);
    void normal(Object_3D *obj, Isect *hit, Point P, Point N);

    static Tri_3D *makeTri(Vec *point);

    static void invertMatrix(Vec in[3], Vec out[3]);
    static void checkTri(Vec *point);
};