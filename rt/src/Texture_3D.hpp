#pragma once
//#include "Surface_3D.hpp"
#include "Turbulence_3D.hpp"
#include "Wave_3D.hpp"

class Surface_3D;
//class Turbulence;

class Texture {
    public:
    Texture(){}
    ~Texture(){}

    Flt (*func)(const Point, const Texture&);    /* returns 0.0..1.0 */
    Flt blur;        /* % blending between layers */
    Flt fuzz;        /* white noise blending */
    Flt r1, r2;         /* just because */
    int terms;          /* for tex_noise() */
    Vec trans,
        scale;
    Surface_3D *surf[2];
    Turbulence *turbulence;
    Wave *waves;
};