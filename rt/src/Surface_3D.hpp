#pragma once
#include "defs.hpp"
#include "Texmap_3D.hpp"

typedef struct t_surface {
    Color   diff;            /* diffuse */
    Texmap *tm_diff;
    Color   spec;            /* specular (reflected) */
    Texmap *tm_spec;
    Color   amb;             /* ambient */
    Texmap *tm_amb;
    Flt     shine;           /* specular spot exponent */
    Color   cshine;          /* spec spot color */
    Color   trans;           /* transparency */
    Texmap *tm_trans;
    Flt     ior;             /* index of refraction */
    Flt     fuzz;            /* surface fuzz */
    int     flags;           /* is this surface valid for shadow caching */
    struct t_texture *tex;   /* ptr for color texture */
    struct t_bump    *bump;  /* ptr for surface normal texture */
    Matrix  matrix;          /* transformation matrix */
} Surface;