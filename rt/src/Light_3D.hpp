#pragma once

#include "Object_3D.hpp"
#include "Color.hpp"
//#include "defs.hpp"

class Light_3D {
  public:
    Light_3D()
    : radius(0.0),    /* radius/spherical, max_angle/spotlight */
      min_angle(0.0), /* angles for spot lights */
      max_angle(0.0),
      type(0),      /* what type is this? */
      illum(0),     /* how does the light fall off? */
      flag(0),      /* noshadows? nospec? */
      samples(0),   /* num samples for spherical light */
      // light_obj_cache(nullptr), // Used by shadow
      next(nullptr) /* next light in list */
    {
        for(int i = 0; i < MAXLEVEL; i++) light_obj_cache[i] = nullptr;
    }
    ~Light_3D() {}

    Vec    position;
    Vec    dir;
    Color  color;
    double radius;    /* radius/spherical, max_angle/spotlight */
    double min_angle, /* angles for spot lights */
           max_angle;
    short  type;      /* what type is this? */
    short  illum;     /* how does the light fall off? */
    short  flag;      /* noshadows? nospec? */
    short  samples;   /* num samples for spherical light */

    Object_3D *light_obj_cache[MAXLEVEL]; // Used by shadow
    Light_3D  *next; /* next light in list */

    static Light_3D *light_head;
};

typedef Light_3D Light;
