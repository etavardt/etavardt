#pragma once
/*
�������������������������������������������
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�                    Defs.C = the center of all confusion                 �
�                                                                         �
�       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     �
�                                                                         �
�       ALL RIGHTS RESERVED.   This software is published, but is NOT     �
�         Public Domain and remains the propery of ALGORITHM, Inc.,       �
�   Christopher D. Watkins and Stephen B. Coy.  This software may not be  �
�  reproduced or integrated into other packages without the prior written �
�          consent of Christopher D. Watkins and Stephen B. Coy.          �
�                                                                         �
�                          Requires: config.h                             �
�                                                                         �
�������������������������������������������
*/
#ifndef STRUCT_DEFS_HPP
#define STRUCT_DEFS_HPP

#include "String.hpp"
#include "BobMath.hpp"
#include "Color.hpp"
#include "defs.hpp"
#include <cstdio>

class Object_3D;
//class Color;
/*
    typedefs for the world
*/

/*----------------------------------------------------------------------*/

#define RayPoint(ray, t, point) VecAddS(t, (ray)->D, (ray)->P, point)

typedef struct t_infile {
    String file_name;
    FILE *fp;
    long line;
    struct t_infile *next;
} Infile;

typedef struct t_stack {
    Infile *what;
    struct t_stack *prev;
} Stack;

// typedef struct t_light {
//     Vec position;
//     Vec dir;
//     Color color;
//     double radius;    /* radius/spherical, max_angle/spotlight */
//     double min_angle, /* angles for spot lights */
//         max_angle;
//     short type;    /* what type is this? */
//     short illum;   /* how does the light fall off? */
//     short flag;    /* noshadows? nospec? */
//     short samples; /* num samples for spherical light */
//     struct t_object *light_obj_cache[MAXLEVEL];
//     struct t_light *next; /* next light in list */
// } Light;

typedef struct t_viewpoint {
    Vec view_from;
    Vec view_at;
    Vec view_up;
    double view_angle_x;
    double view_angle_y;
    double view_aspect;
} Viewpoint;

typedef struct t_camera {
    short projection;
    Vec lens_i, /* vectors across lens */
        lens_j;
    double aperture,     /* radius of lens */
        focal_length; /* how far away are we focussed */
    short samples;    /* num samples for non-pinhole camera */
} Camera;

// typedef struct t_pixel {
//     unsigned char r, g, b, q;
// } Pixel;

typedef struct t_transform {
    Matrix mat;
    struct t_transform *next;
} Transform;

typedef Color Palette[256];

typedef struct t_background {
    Color color;
    Vec up;
    Palette pal;
} Background;

/* preprocessor macro structure */
typedef struct t_macro {
    char *macro,          /* define'd word */
        *text;            /* text to replace macro with */
    int mlen,             /* length of macro */
        tlen;             /* length of text */
    struct t_macro *next; /* stack link */
} Macro;

//TODO: TCE:maybe move to bound.hpp
typedef struct t_compositedata {
    unsigned long   size;
    Object_3D          *children;
} CompositeData;

#endif //STRUCT_DEFS_HPP