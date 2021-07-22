#pragma once
#include "Object_3D.hpp"
#include "defs.hpp"
#include "Ray_3D.hpp"
#include "Point_3D.hpp"

//class Object_3D;
class Isect;
//class Ray;

class Tri_3D: public Object_3D {
    public:
    Tri_3D();
    ~Tri_3D();

    int intersect(Ray *ray, Isect &hit);
    void normal(Isect &hit, Point &P, Vec &N);

    static Tri_3D *makeTri(Vec *point);//Vec array

    static void invertMatrix(const Vec in[3], Vec out[3]);
    static void checkTri(Vec *point);//Vec array
};
