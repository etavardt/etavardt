#pragma once
#include "Object_3D.hpp"

class Sphere_3D: public Object_3D {
    public:
    Sphere_3D();
    ~Sphere_3D();

    int intersect(Ray *ray, Isect &hit) override;
    void normal(Isect &hit, Point &P, Vec &N) override;

    static Sphere_3D *makeSphere(Vec &pos, double radius, double fuzzy);

};
