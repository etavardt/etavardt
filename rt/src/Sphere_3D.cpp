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

#include "Sphere_3D.hpp"
#include "Bob.hpp"
#include "Isect_3D.hpp"
#include "Stats.hpp"
#include "defs.hpp"
#include "extern.hpp"
#include "proto.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>

typedef struct t_spheredata {
    Vec sph_center;
    Flt sph_radius;  /* actually store 1/radius */
    Flt sph_fuzzy;   /* RAND_MAX/fuzzy part of radius */
    Flt sph_radius2; /* radius squared */
} SphereData;
/*
ObjectProcs SphereProcs = {
    SphereIntersect,
    SphereNormal,
};
*/

Sphere_3D::Sphere_3D() : Object_3D() {
    o_type = T_SPHERE;
    o_surf = CurrentSurface;
}
Sphere_3D::~Sphere_3D() {
    Object_3D::~Object_3D();
    if (o_data != NULL) {
        delete (SphereData*)o_data;
    }
}

int Sphere_3D::intersect(Object_3D *obj, Ray *ray, Isect &hit) {
    Flt b, disc, t, t0, t1, dot_vv;
    Point V, P;
    SphereData *sp;

    sp = (SphereData *)obj->o_data;

    if (obj->o_type == T_FUZZY) { /* its a fuzz ball */
        Flt tmp;

        tmp = 1.0 / sp->sph_radius + (Flt)rand() / sp->sph_fuzzy;
        sp->sph_radius2 = tmp * tmp;
    }

    VecSub((sp->sph_center), ray->P, V);

    b = VecDot(V, ray->D);

    dot_vv = VecDot(V, V);
    disc = b * b - dot_vv + (sp->sph_radius2);

    if (disc < 0.0)
        return 0;

    disc = sqrt(disc);

    t0 = b - disc;
    t1 = b + disc;

    if (t0 > rayeps) { /* possible winner */
        if (!obj->clips) {
            t = t0;
        } else {
            RayPoint(ray, t0, P);
            if (clip_check(obj->clips, P)) {
                t = t0;
            } else { /* well, try t1 */
                RayPoint(ray, t1, P);
                if (clip_check(obj->clips, P)) {
                    t = t1;
                } else {
                    return 0;
                }
            }
        }
    } else if (t1 > rayeps) {
        if (!obj->clips) {
            t = t1;
        } else {
            RayPoint(ray, t1, P);
            if (clip_check(obj->clips, P)) {
                t = t1;
            } else {
                return 0;
            }
        }
    } else {
        return 0;
    }

    hit.isect_t = t;
    hit.isect_enter = dot_vv > sp->sph_radius2 + rayeps ? 1 : 0;
    hit.isect_self = hit.isect_enter ? NULL : obj;
    hit.isect_prim = obj;
    hit.isect_surf = obj->o_surf;

    return 1;
}

void Sphere_3D::normal(Object_3D *obj, Isect &hit, Point P, Point N) {
    SphereData *sp;

    sp = (SphereData *)obj->o_data;

    VecSub(P, sp->sph_center, N);
    VecNormalize(N);
}

Sphere_3D *Sphere_3D::makeSphere(Vec pos, Flt radius, Flt fuzzy) {
    Sphere_3D *tmp;
    int i;
    SphereData *sp;

    tmp = new Sphere_3D();
    Stats::trackMemoryUsage(sizeof(Sphere_3D));
    Bob::getApp().parser.ptrchk(tmp, "sphere object");
    if (fuzzy > rayeps) {
        tmp->o_type = T_FUZZY;
    } else {
        tmp->o_type = T_SPHERE;
    }
    //    tmp->o_procs = &SphereProcs;
    //    tmp->o_surf = CurrentSurface;

    if (ClipTop) {
        tmp->clips = ClipTop;
        ClipTop = GlobalClipTop->clip;
    } else {
        tmp->clips = NULL;
    }

    sp = new SphereData();
    Stats::trackMemoryUsage(sizeof(SphereData));
    Bob::getApp().parser.ptrchk(tmp, "sphere data");
    VecCopy(pos, sp->sph_center);
    radius = ABS(radius);
    if (radius < rayeps)
        radius = rayeps;
    sp->sph_radius = 1.0 / radius;
    sp->sph_radius2 = radius * radius;
    if (tmp->o_type == T_FUZZY) {
        sp->sph_fuzzy = (Flt)RAND_MAX / fuzzy;
        radius += fuzzy; /* for calcing bounding slabs */
    }
    tmp->o_data = (void *)sp;

    /*
     * figure out dmin and dmax values for
     * each of the slabs...
     */

    for (i = 0; i < NSLABS; i++) {
        tmp->o_dmin[i] = VecDot(sp->sph_center, Slab[i]) - ABS(radius);
        tmp->o_dmax[i] = VecDot(sp->sph_center, Slab[i]) + ABS(radius);
    }

    if (tmp->clips) {
        bound_opt(tmp);
    }

    return tmp;
}
