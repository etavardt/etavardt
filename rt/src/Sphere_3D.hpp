#pragma once
#include "Object_3D.hpp"

class Sphere_3D: public Object_3D {
    public:
    Sphere_3D();
    ~Sphere_3D();

    int intersect(Object_3D *obj, Ray *ray, Isect &hit);
    void normal(Object_3D *obj, Isect &hit, Point &P, Vec &N);

    static Sphere_3D *makeSphere(Vec pos, double radius, double fuzzy);

};