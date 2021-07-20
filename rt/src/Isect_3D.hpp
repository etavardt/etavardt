#pragma once
#include "defs.hpp"
class Object_3D;
class Surface_3D;

class Isect {
    public:
    double      isect_t;       /* distance to intersection */
    int         isect_enter;   /* entering? ie hit front? */
    Object_3D  *isect_prim;    /* object we hit */
    Surface_3D *isect_surf;    /* surface def of hit object */
    Object_3D  *isect_self;    /* pointer to self for queue elimination */

    Isect &operator=(const Isect &isect);
};
