#include "Isect_3D.hpp"

Isect &Isect::operator=(const Isect isect) {
    this->isect_t = isect.isect_t;         /* distance to intersection */
    this->isect_enter = isect.isect_enter; /* entering? ie hit front? */
    this->isect_prim = isect.isect_prim;   /* object we hit */
    this->isect_surf = isect.isect_surf;   /* surface def of hit object */
    this->isect_self = isect.isect_self;   /* pointer to self for queue elimination */

    return *this;
}
