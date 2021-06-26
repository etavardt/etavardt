#include <iostream>
#include "Object_3D.hpp"
#include "Exception.hpp"

Object_3D::~Object_3D() {
}

int  Object_3D::intersect(Object_3D *a, Ray *b, Object_3D::Isect *c) {
    std::cerr << "Called non-existant intersect routine for bounding box, dying..." << std::endl;
    throw Exception("Thrown from Object_3D::Intersect");
    return 1;       /* keep lint/compilers quiet */
}

void Object_3D::normal(Object_3D *a, Object_3D::Isect *b, Flt *c, Flt *d) {
    std::cerr << "Called non-existant intersect routine for bounding box, dying..." << std::endl;
    throw Exception("Thrown from Object_3D::Normal");
}
