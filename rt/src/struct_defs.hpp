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

#include <stdio.h>
#include "String.hpp"
#include "defs.hpp"
//#include "config.hpp"
/*
    typedefs for the world
*/


/*----------------------------------------------------------------------*/
/* TODO: TCE Remove
typedef struct t_Ray {
    Point P;
    Point D;
} Ray;
*/
#define RayPoint(ray,t,point)    VecAddS(t,(ray)->D,(ray)->P,point)

/* TODO: TCE Remove
/*
    texture map structure
* /
typedef struct t_texmap {
    Vec     position,       /* upper left hand corner of image * /
        normal,         /* same as projection direction * /
        across,         /* across top of image to upper right * /
        down;           /* down to lower left * /
    Flt     scale;          /* defaults to 1.0, #units across full image * /
    int     xres, yres;     /* image size * /
    unsigned char
        **red,
        **grn,
        **blu;
} Texmap;
*/
/* surface structure */
/* TODO: TCE Remove
typedef struct t_surface {
    Color   diff;                   /* diffuse * /
    Texmap  *tm_diff;
    Color    spec;            /* specular (reflected) * /
    Texmap  *tm_spec;
    Color    amb;            /* ambient * /
    Texmap  *tm_amb;
    Flt     shine;                  /* specular spot exponent * /
    Color   cshine;                 /* spec spot color * /
    Color   trans;                  /* transparency * /
    Texmap  *tm_trans;
    Flt    ior;            /* index of refraction * /
    Flt     fuzz;                   /* surface fuzz * /
    int     flags;                  /* is this surface valid for shadow caching * /
    struct t_texture    *tex;    /* ptr for color texture * /
    struct t_bump           *bump;  /* ptr for surface normal texture * /
    Matrix  matrix;                 /* transformation matrix * /
} Surface;

typedef struct t_turbulence {
    int     terms;          /* # of terms in the series * /
    Vec     trans,          /* pre-process numbers * /
        scale;
    Flt     amp;            /* post turbulence amplification * /
} Turbulence;

typedef struct t_wave {
    Vec    center;
    Flt    wavelength,
        amp,        /* should be about 0 to .6 * /
        damp,        /* damping per wavelength * /
        phase;        /* wavelength offset * /
    struct t_wave    *next;    /* next wave in line * /
} Wave;
/*
typedef struct t_texture {
    Flt    (*func)(Point, t_texture*);    /* returns 0.0..1.0 * /
    Flt    blur;        /* % blending between layers * /
    Flt    fuzz;        /* white noise blending * /
    Flt     r1, r2;         /* just because * /
    int     terms;          /* for tex_noise() * /
    Vec     trans,
        scale;
    Surface *surf[2];
    Turbulence      *turbulence;
    Wave    *waves;
} Texture;

typedef struct t_bump {
    Vec    trans,        /* apply to resulting normal * /
        scale;
    Turbulence      *turbulence;
    Wave            *waves;
} Bump;
*/
typedef struct t_infile {
//    char            *file_name;
    String          file_name;
    FILE            *fp;
    long            line;
    struct t_infile *next;
} Infile;

typedef struct t_stack {
    Infile        *what;
    struct t_stack    *prev;
} Stack;

typedef struct t_light {
    Vec    position;
    Vec    dir;
    Color    color;
    Flt     radius;         /* radius/spherical, max_angle/spotlight */
    Flt     min_angle,      /* angles for spot lights */
        max_angle;
    short    type;        /* what type is this? */
    short    illum;        /* how does the light fall off? */
    short   flag;           /* noshadows? nospec? */
    short   samples;        /* num samples for spherical light */
    struct t_object *light_obj_cache[MAXLEVEL];
    struct t_light  *next;  /* next light in list */
} Light;

typedef struct t_viewpoint {
    Vec    view_from;
    Vec    view_at;
    Vec    view_up;
    Flt    view_angle_x;
    Flt    view_angle_y;
    Flt    view_aspect;
} Viewpoint;

typedef struct t_camera {
    short   projection;
    Vec    lens_i,        /* vectors across lens */
        lens_j;
    Flt    aperture,    /* radius of lens */
        focal_length;   /* how far away are we focussed */
    short   samples;        /* num samples for non-pinhole camera */
} Camera;
/* TODO: TCE Remove:
typedef struct t_clip {
    Vec     center,
        normal,
        apex, base;
    Flt     radius1, radius2, length;
    int     type;
    struct t_clip   *next;
} Clip;

typedef struct t_global_clip {
    Clip                    *clip;
    struct t_global_clip    *next;
} GlobalClip;
*/
// TODO: TCE Remove: typedef t_object Object;
/*
typedef struct t_isect {
    Flt     isect_t;    /* distance to intersection * /
    int     isect_enter;    /* entering? ie hit front? * /
    Object     *isect_prim;    /* object we hit * /
    Surface *isect_surf;    /* surface def of hit object * /
    Object    *isect_self;    /* pointer to self for queue elimination * /
} Isect;
*/
/* TODO: TCE Remove
typedef struct t_object {
    unsigned short     o_type;
    Flt             o_dmin[NSLABS];
    Flt             o_dmax[NSLABS];
    struct t_objectprocs {
        int     (*intersect) (t_object*, Ray*, Isect*);
        void    (*normal) (t_object*, Isect*, Flt*, Flt*);
    } * o_procs;
    Surface         *o_surf;
    void            *o_data;
    Clip            *clips;
    struct t_object *next;          / * next object in original list, sibling * /
} Object;
*/
/*
typedef struct t_compositedata {
    unsigned long   size;
    Object          *children;
} CompositeData;
*/
//TODO: TCE Remove typedef struct t_object::t_objectprocs ObjectProcs;

typedef struct t_pixel {
    unsigned char r, g, b, q;
} Pixel;

typedef struct t_transform {
    Matrix                  mat;
    struct t_transform      *next;
} Transform;

typedef unsigned char   Palette[256][3];

typedef struct t_background {
    Color   color;
    Vec     up;
    Palette pal;
} Background;

/* preprocessor macro structure */
typedef struct t_macro {
    char            *macro,         /* define'd word */
            *text;          /* text to replace macro with */
    int             mlen,           /* length of macro */
            tlen;           /* length of text */
    struct t_macro  *next;          /* stack link */
} Macro;

#endif //STRUCT_DEFS_HPP