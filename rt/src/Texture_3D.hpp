#pragma once
#include "Surface_3D.hpp"
#include "Turbulence_3D.hpp"
#include "Wave_3D.hpp"

typedef struct t_texture {
    Flt (*func)(Point, t_texture*);    /* returns 0.0..1.0 */
    Flt blur;        /* % blending between layers */
    Flt fuzz;        /* white noise blending */
    Flt r1, r2;         /* just because */
    int terms;          /* for tex_noise() */
    Vec trans,
        scale;
    Surface *surf[2];
    Turbulence *turbulence;
    Wave *waves;
} Texture;