#pragma once
/*
***************************************************************************
*                                                                         *
*                             Bob Ray Tracer                              *
*                                                                         *
*    Extern.H = external defs for all globals, also include prototypes    *
*                                                                         *
*       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     *
*                                                                         *
*       ALL RIGHTS RESERVED.   This software is published, but is NOT     *
*         Public Domain and remains the propery of ALGORITHM, Inc.,       *
*   Christopher D. Watkins and Stephen B. Coy.  This software may not be  *
*  reproduced or integrated into other packages without the prior written *
*          consent of Christopher D. Watkins and Stephen B. Coy.          *
*                                                                         *
*                            Requires: proto.h                            *
*                                                                         *
***************************************************************************
*/

#include "Vector_3D.hpp"

// Bob, Parser, stats
// extern int Xresolution;
// extern int Yresolution;

// Bob, Parser, stats, PicFile_3D, Screen_3D*
// extern int start_line, stop_line;

//Bob, Bound_3D, Parser*
extern int bunching;

//Bob, screen_3D*, Parser*
extern int antialias;
//screen_3D*, Parser
extern int jitter;
// Parser, Screen_3D*
extern int adapt_dist;

// Bob, Parser, shade*
extern int    no_shadows;
// Parser, shadow*
extern int    caustics;
// Parser, shadow, shade
extern int    exp_trans;
// Screen_3D, shade
extern int    fuzzy_ray;

// pqueue, stats
extern unsigned long maxQueueSize;
extern unsigned long totalQueues;
extern unsigned long totalQueueResets;
// inter, stats
extern unsigned long nChecked;
// inter
extern unsigned long nEnqueued;
// shadow, stats
extern unsigned long nShadowCacheHits;

// Bob, PicFile_3D
//extern int resume;

// Not used
extern int gr_mode;

// shade
//extern double minweight;

// Bob, Parser, shade, stats, trace
// extern int maxlevel;

// stats, trace
// extern int deepest;
// extern unsigned long nRays;

// shadow, stats
// extern unsigned long nShadows;

// shade, stats
// extern unsigned long nReflected;
// extern unsigned long nRefracted;

// Cone_3D, Poly_3D, Sphere_3D, Tri_3D so maybe put it in Object_3D
// extern Vec     Slab[];
