#pragma once

#include "Vector_3D.hpp"
#include "Screen_3D.hpp"

struct Viewpoint_3D {
    Vec view_from;
    Vec view_at;
    Vec view_up;
    double view_angle_x;
    double view_angle_y;
    double view_aspect;

    Viewpoint_3D() : view_aspect(-1.0) {}
};
typedef Viewpoint_3D Viewpoint;

struct Camera_3D {
    short projection;
    Vec lens_i, /* vectors across lens */
        lens_j;
    double aperture,     /* radius of lens */
           focal_length; /* how far away are we focussed */
    short samples;    /* num samples for non-pinhole camera */

    Viewpoint_3D eye; // The camera has a veiwpoint orientation
    Screen_3D screen; // The camera has a screen/film
    Camera_3D() : screen(*this), aperture(-1.0), focal_length(0.0) {}
};
typedef Camera_3D Camera;
