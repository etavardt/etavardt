#include <iostream>
#include "Object_3D.hpp"
//TODO: TCE throw exceptions
int  Object_3D::Intersect(Object_3D *a, Ray *b, Object_3D::Isect *c) {
    std::cerr << "Called non-existant intersect routine for bounding box, dying..." << std::endl;
    exit(1);
    return 1;       /* keep lint/compilers quiet */
}

void Object_3D::Normal(Object_3D *a, Object_3D::Isect *b, Flt *c, Flt *d) {
    std::cerr << "Called non-existant intersect routine for bounding box, dying..." << std::endl;
    exit(1);
}
