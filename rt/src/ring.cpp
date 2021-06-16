/*
�������������������������������������������
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�                       Ring.C = the ring primative                       �
�                                                                         �
�       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     �
�                                                                         �
�       ALL RIGHTS RESERVED.   This software is published, but is NOT     �
�         Public Domain and remains the propery of ALGORITHM, Inc.,       �
�   Christopher D. Watkins and Stephen B. Coy.  This software may not be  �
�  reproduced or integrated into other packages without the prior written �
�          consent of Christopher D. Watkins and Stephen B. Coy.          �
�                                                                         �
�                       Requires: defs.h, extern.h                        �
�                                                                         �
�������������������������������������������
*/

#include <cstdio>
#include <cmath>
#include "defs.hpp"
#include "extern.hpp"
#include "proto.hpp"

typedef struct t_ringdata {
    Vec    ring_center;
    Vec    ring_normal;
    Flt    D;
    Flt    min_radius, max_radius;    /* actually store squared values */
} RingData;

ObjectProcs RingProcs = {
    RingIntersect,
    RingNormal,
};

int RingIntersect(Object    *obj, Ray    *ray, Isect    *hit)
{
    RingData    *rp;
    Flt        Vprd, Vpro, t, rad;
    Vec        point;

    rp = (RingData *)obj->o_data;    /* point to ring data */

    Vprd = VecDot(rp->ring_normal, ray->D);

    if(Vprd == 0) {            /* ray in plane */
        return 0;
    }

    Vpro = -(VecDot(rp->ring_normal, ray->P) + rp->D);

    t = Vpro/Vprd;

    if(t < rayeps) {        /* intersect behind us or self */        
        return 0;
    }

    RayPoint(ray, t, point);

    /* if clipping planes and doesn't pass, bail */
    if(obj->clips && !clip_check(obj->clips, point)) {
        return 0;
    }

    /* calc dist to ring center */
    VecSub(rp->ring_center, point, point);
    rad = VecDot(point, point);    /* calc dist^2 */

    if(rad>rp->max_radius || rad<rp->min_radius) {
        return 0;        /* missed the ring */
    }

    hit->isect_t = t;
    hit->isect_prim = obj;
    hit->isect_self = obj;        /* rings are not self intersecting */
    hit->isect_surf = obj->o_surf;

    return 1;
}

void    RingNormal(Object *obj, Isect *hit, Point P, Point N)
{
    RingData    *rp;
    rp = (RingData *)obj->o_data;

    VecCopy(rp->ring_normal, N);    /* already normalized */
}

Object *MakeRing(Vec pos, Vec norm, Flt min_rad, Flt max_rad)
{
    Object        *tmp;
    int        i;
    RingData    *rp;
    Flt             size;

    tmp = (Object *)vmalloc(sizeof(Object));
    ptrchk(tmp, "ring object");
    tmp->o_type = T_RING;
    tmp->o_procs = &RingProcs;
    tmp->o_surf = CurrentSurface;

    if(ClipTop) {
        tmp->clips = ClipTop;
        ClipTop = GlobalClipTop->clip;
    } else {
        tmp->clips = NULL;
    }

    rp = (RingData *)vmalloc(sizeof(RingData));
    ptrchk(rp, "ring data");
    VecCopy(pos, rp->ring_center);
    VecCopy(norm, rp->ring_normal);
    VecNormalize(rp->ring_normal);
    rp->min_radius = min_rad * min_rad;
    rp->max_radius = max_rad * max_rad;
    rp->D = -VecDot(rp->ring_center, rp->ring_normal);
    tmp->o_data = (void *)rp;

    /*
     * figure out dmin and dmax values for 
     * each of the slabs...
     */

    max_rad = ABS(max_rad);        /* just to be sure... */
    for(i=0; i<NSLABS; i++) {
        size = VecDot(Slab[i], rp->ring_normal);
        size = max_rad * sqrt(1.0 - size*size);
        tmp->o_dmin[i] = pos[i] - size;
        tmp->o_dmax[i] = pos[i] + size;
    }

    if(tmp->clips) {
        bound_opt(tmp);
    }

    return tmp;
}


