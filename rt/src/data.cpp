/*
�������������������������������������������
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�                    Data.C = aka - where the globals are                 �
�                                                                         �
�       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     �
�                                                                         �
�       ALL RIGHTS RESERVED.   This software is published, but is NOT     �
�         Public Domain and remains the propery of ALGORITHM, Inc.,       �
�   Christopher D. Watkins and Stephen B. Coy.  This software may not be  �
�  reproduced or integrated into other packages without the prior written �
�          consent of Christopher D. Watkins and Stephen B. Coy.          �
�                                                                         �
�                           Requires: defs.h                              �
�                                                                         �
�������������������������������������������
*/

#include <cstdio>
#include "String.hpp"
#include "defs.hpp"
#include "struct_defs.hpp"
#include "Object_3D.hpp"
#include "extern.hpp"

int             yylinecount = 1;
Viewpoint     Eye;
Camera        camera;
int         Xresolution = 320;    /* why?  why not? */
int         Yresolution = 200;
int             start_line, stop_line;
int             bunching = BUNCHINGFACTOR;

int        antialias = 0;        /* default to none */
int        jitter = 0;        /* none */
int        adapt_dist = 8;        /* threshold for adaptive sampling */

Light           *light_head = NULL;
int        nLights = 0;        /* it's a dark world out there */
int             no_shadows = 0;         /* shadows by default */
int             caustics = 0;           /* no fake caustics */
int             exp_trans = 1;          /* default to exponential transparency */
int             fuzzy_ray;

Background      background;             /* background color structure */
Color           Ambient;                /* global ambient light */
Color        HazeColor;        /* color of haze */
Flt        HazeDensity;        /* exponential factor */
Surface         *CurrentSurface = NULL; /* surface at current top of stack */
Stack        *SurfTop = NULL;    /* points to top of surface stack */
Transform       *TransTop = NULL;       /* top of transformation stack */
Clip            *ClipTop = NULL;        /* current clipping list */
GlobalClip      *GlobalClipTop = NULL;  /* current global clip list */
long            nPrims = 0;
Flt        rayeps = 1e-6;
//char        *Progname;
//char            Infilename[256];
String          Infilename;
Stack           *InfileTop = NULL;      /* points to top of input file stack */
Object          *Root = NULL;

Flt             minweight = MINWEIGHT;
int        maxlevel = MAXLEVEL;
int             deepest = 0;            /* deepest level reached */
unsigned long   nRays = 0;
unsigned long   nShadows = 0;
unsigned long   nReflected = 0;
unsigned long   nRefracted = 0;
unsigned long   maxQueueSize = 0;
unsigned long   totalQueues = 0;
unsigned long   totalQueueResets = 0;
int             tickflag = 1;           /* also set in main() */
int             resume = 0;             /* also set in main() */
int             gr_mode = 0;
unsigned long   nChecked = 0;
unsigned long   nEnqueued = 0;
unsigned long   nShadowCacheHits = 0;
//unsigned long   MemAllocated = 0;

FILE     *yyin;         /* pointer to input file */
int      cur_token;     /* parser token stuff */
char     cur_text[256];
Flt      cur_value;

Vec    Slab[] = {
    {1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 0.0, 1.0},
    {1.0, 1.0, 0.0},
    {1.0, 0.0, 1.0},
    {0.0, 1.0, 1.0}
};

