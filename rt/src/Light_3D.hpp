#pragma once

#include "Color.hpp"
//#include "defs.hpp"
class Object_3D;

class Light {
    public:
    Light(){}
    ~Light(){}
    
    Vec position;
    Vec dir;
    Color color;
    Flt radius;    /* radius/spherical, max_angle/spotlight */
    Flt min_angle, /* angles for spot lights */
        max_angle;
    short type;    /* what type is this? */
    short illum;   /* how does the light fall off? */
    short flag;    /* noshadows? nospec? */
    short samples; /* num samples for spherical light */
    Object_3D *light_obj_cache[MAXLEVEL];
    Light *next; /* next light in list */
} ;
