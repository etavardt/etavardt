#pragma once
/*
�������������������������������������������
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�    Extern.H = external defs for all globals, also include prototypes    �
�                                                                         �
�       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     �
�                                                                         �
�       ALL RIGHTS RESERVED.   This software is published, but is NOT     �
�         Public Domain and remains the propery of ALGORITHM, Inc.,       �
�   Christopher D. Watkins and Stephen B. Coy.  This software may not be  �
�  reproduced or integrated into other packages without the prior written �
�          consent of Christopher D. Watkins and Stephen B. Coy.          �
�                                                                         �
�                            Requires: proto.h                            �
�                                                                         �
�������������������������������������������
*/

//#include "Object_3D.hpp"
//#include "proto.hpp"
#include "struct_defs.hpp"
#include "Surface_3D.hpp"
#include "Clip.hpp"
#include "Object_3D.hpp"

extern Viewpoint Eye;
extern Camera    camera;

extern int Xresolution;
extern int Yresolution;

extern int start_line, stop_line;
extern int bunching;

extern int yylinecount;

extern int antialias;
extern int jitter;
extern int adapt_dist;

extern Light *light_head;
extern int    nLights;
extern int    no_shadows;
extern int    caustics;
extern int    exp_trans;
extern int    fuzzy_ray;

extern Background  background;
extern Color       Ambient;
extern Color       HazeColor;
extern Flt         HazeDensity;
extern Surface    *CurrentSurface;
extern Stack      *SurfTop;
extern Stack      *InfileTop;
extern Transform  *TransTop;
extern Clip       *ClipTop;
extern GlobalClip *GlobalClipTop;
extern long        nPrims;
extern Flt         rayeps;
extern char       *Progname;
extern char        Infilename[];

extern unsigned long maxQueueSize;
extern unsigned long totalQueues;
extern unsigned long totalQueueResets;
extern unsigned long nChecked;
extern unsigned long nEnqueued;
extern unsigned long nShadowCacheHits;

extern int tickflag;
extern int resume;
extern int gr_mode;

extern Flt minweight;
extern int maxlevel;
extern int deepest;

extern unsigned long nRays;
extern unsigned long nShadows;
extern unsigned long nReflected;
extern unsigned long nRefracted;
extern unsigned long MemAllocated;

extern Vec     Slab[];
extern Object *Root;
// TODO: TCE Remove: extern ObjectProcs    NullProcs;

extern char _Copyright[];
extern char _Program[];
extern char _Version[];
extern char _Date[];

extern FILE *yyin;
extern int   cur_token;
extern char  cur_text[];
extern Flt   cur_value;
