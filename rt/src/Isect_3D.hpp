#pragma once
#include "defs.hpp"
class Object_3D;
class Surface_3D;
/*
#include "defs.hpp"
#include "Object_3D.hpp"
#include "Surface_3D.hpp"
typedef struct t_isect {
    Flt      isect_t;       /* distance to intersection * /
    int      isect_enter;   /* entering? ie hit front? * /
    t_object  *isect_prim;    /* object we hit * /
    Surface *isect_surf;    /* surface def of hit object * /
    t_object  *isect_self;    /* pointer to self for queue elimination * /
} Isect;
*/
class Isect {
    public:
    Flt         isect_t;       /* distance to intersection */
    int         isect_enter;   /* entering? ie hit front? */
    Object_3D  *isect_prim;    /* object we hit */
    Surface_3D *isect_surf;    /* surface def of hit object */
    Object_3D  *isect_self;    /* pointer to self for queue elimination */

    Isect &operator=(const Isect isect);
};
