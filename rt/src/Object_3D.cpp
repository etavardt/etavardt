#include "Object_3D.hpp"

#include <iostream>

#include "Vector_3D.hpp"
#include "Exception.hpp"

#include "unused_macros.hpp"

Object_3D *Object_3D::Root;

Vec  Object_3D::Slab[] = {
    {1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 0.0, 1.0},
    {1.0, 1.0, 0.0},
    {1.0, 0.0, 1.0},
    {0.0, 1.0, 1.0}
};

Object_3D::~Object_3D() {}

int Object_3D::intersect(Ray *UNUSED_VAR(b), Isect &UNUSED_VAR(c)) {
    // parameters are not used here for now
    // TCE: As is, unable to create a composite class could cycle threw the children in order find a child that intersects, could it not?
    std::cerr << "Called non-existant intersect routine for bounding box, dying..." << std::endl;
    throw Exception("Thrown from Object_3D::intersect");
    return 1; /* keep lint/compilers quiet */
}

void Object_3D::normal(Isect &UNUSED_VAR(b), Point &UNUSED_VAR(c), Vec &UNUSED_VAR(d)) {
    // parameters are not used here for now
    // TCE: As is, unable to create a composite class could cycle threw the children in order find a childs normal that should be used, could it not?
    std::cerr << "Called non-existant normal routine for bounding box, dying..." << std::endl;
    throw Exception("Thrown from Object_3D::normal");
}

// Isect &Object_3D::Isect::operator=(const Isect isect) {
//     this->isect_t = isect.isect_t;         /* distance to intersection */
//     this->isect_enter = isect.isect_enter; /* entering? ie hit front? */
//     this->isect_prim = isect.isect_prim;   /* object we hit */
//     this->isect_surf = isect.isect_surf;   /* surface def of hit object */
//     this->isect_self = isect.isect_self;   /* pointer to self for queue elimination */

//     return *this;
// }
