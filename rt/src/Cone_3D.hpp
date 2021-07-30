#pragma once
#include "Object_3D.hpp"

class Cone_3D: public Object_3D {
    public:
    Cone_3D();
    ~Cone_3D();

    int intersect(Ray *ray, Isect &hit) override;
    void normal(Isect &hit, Point &P, Vec &N) override;

    static Cone_3D *makeCone(Vec &basepoint, double baseradius, Vec &apexpoint, double apexradius);
};
