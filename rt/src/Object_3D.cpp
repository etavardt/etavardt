#include "Object_3D.hpp"
#include "Exception.hpp"
#include <iostream>

Object_3D::~Object_3D() {}

int Object_3D::intersect(Object_3D *a, Ray *b, Object_3D::Isect &c) {
    std::cerr << "Called non-existant intersect routine for bounding box, dying..." << std::endl;
    throw Exception("Thrown from Object_3D::intersect");
    return 1; /* keep lint/compilers quiet */
}

void Object_3D::normal(Object_3D *a, Object_3D::Isect &b, Flt *c, Flt *d) {
    std::cerr << "Called non-existant intersect routine for bounding box, dying..." << std::endl;
    throw Exception("Thrown from Object_3D::normal");
}

Isect &Object_3D::Isect::operator=(const Isect isect) {
    this->isect_t = isect.isect_t;         /* distance to intersection */
    this->isect_enter = isect.isect_enter; /* entering? ie hit front? */
    this->isect_prim = isect.isect_prim;   /* object we hit */
    this->isect_surf = isect.isect_surf;   /* surface def of hit object */
    this->isect_self = isect.isect_self;   /* pointer to self for queue elimination */

    return *this;
}
