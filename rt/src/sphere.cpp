/*
�������������������������������������������
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�                    Sphere.C = the sphere primative                      �
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
#include <cstdlib>
#include <cmath>
#include "defs.hpp"
#include "extern.hpp"
#include "proto.hpp"

typedef struct t_spheredata {
    Vec    sph_center;
    Flt     sph_radius;     /* actually store 1/radius */
    Flt     sph_fuzzy;      /* RAND_MAX/fuzzy part of radius */
    Flt     sph_radius2;    /* radius squared */
} SphereData;

ObjectProcs SphereProcs = {
    SphereIntersect,
    SphereNormal,
};

int SphereIntersect(Object *obj, Ray *ray, Isect *hit)
{
    Flt             b, disc, t, t0, t1, dot_vv;
    Point           V, P;
    SphereData    *sp;

    sp = (SphereData *)obj->o_data;

    if(obj->o_type == T_FUZZY) {    /* its a fuzz ball */
        Flt     tmp;

        tmp = 1.0/sp->sph_radius + (Flt)rand()/sp->sph_fuzzy;
        sp->sph_radius2 = tmp*tmp;
    }

    VecSub((sp->sph_center), ray->P, V);

    b = VecDot(V, ray->D);

    dot_vv = VecDot(V, V);
    disc = b * b - dot_vv + (sp->sph_radius2);

    if(disc < 0.0)
        return 0;

    disc = sqrt(disc);

    t0 = b - disc;
    t1 = b + disc;

    if(t0 > rayeps) {       /* possible winner */
        if(!obj->clips) {
            t = t0;
        } else {
            RayPoint(ray, t0, P);
            if(clip_check(obj->clips, P)) {
                t = t0;
            } else {        /* well, try t1 */
                RayPoint(ray, t1, P);
                if(clip_check(obj->clips, P)) {
                    t = t1;
                } else {
                    return 0;
                }
            }
        }
    } else if(t1 > rayeps) {
        if(!obj->clips) {
            t = t1;
        } else {
            RayPoint(ray, t1, P);
            if(clip_check(obj->clips, P)) {
                t = t1;
            } else {
                return 0;
            }
        }
    } else {
        return 0;
    }

    hit->isect_t = t;
    hit->isect_enter = dot_vv > sp->sph_radius2 + rayeps ? 1 : 0;
    hit->isect_self = hit->isect_enter ? NULL : obj;
    hit->isect_prim = obj;
    hit->isect_surf = obj->o_surf;

    return 1;
}

void    SphereNormal(Object *obj, Isect *hit, Point P, Point N)
{
    SphereData    *sp;

    sp = (SphereData *)obj->o_data;

    VecSub(P, sp->sph_center, N);
    VecNormalize(N);
}

Object *MakeSphere(Vec pos, Flt radius, Flt fuzzy)
{
    Object        *tmp;
    int        i;
    SphereData    *sp;

    tmp = (Object *)vmalloc(sizeof(Object));
    ptrchk(tmp, "sphere object");
    if(fuzzy > rayeps) {
        tmp->o_type = T_FUZZY;
    } else {
        tmp->o_type = T_SPHERE;
    }
    tmp->o_procs = &SphereProcs;
    tmp->o_surf = CurrentSurface;

    if(ClipTop) {
        tmp->clips = ClipTop;
        ClipTop = GlobalClipTop->clip;
    } else {
        tmp->clips = NULL;
    }

    sp = (SphereData *) vmalloc (sizeof(SphereData));
    ptrchk(tmp, "sphere data");
    VecCopy(pos, sp->sph_center);
    radius = ABS(radius);
    if(radius < rayeps)
        radius = rayeps;
    sp->sph_radius = 1.0/radius;
    sp->sph_radius2 = radius * radius;
    if(tmp->o_type == T_FUZZY) {
        sp->sph_fuzzy = (Flt)RAND_MAX/fuzzy;
        radius += fuzzy;        /* for calcing bounding slabs */
    }
    tmp->o_data = (void *)sp;

    /*
     * figure out dmin and dmax values for 
     * each of the slabs...
     */
    
    for (i = 0; i < NSLABS; i ++) {
        tmp->o_dmin[i] = VecDot(sp->sph_center, Slab[i]) 
            - ABS(radius);
        tmp->o_dmax[i] = VecDot(sp->sph_center, Slab[i]) 
            + ABS(radius);
    }

    if(tmp->clips) {
        bound_opt(tmp);
    }

    return tmp;
}

