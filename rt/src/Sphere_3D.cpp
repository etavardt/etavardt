/*
***************************************************************************
*                                                                         *
*                             Bob Ray Tracer                              *
*                                                                         *
*                    Sphere.C = the sphere primative                      *
*                                                                         *
*       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     *
*                                                                         *
*       ALL RIGHTS RESERVED.   This software is published, but is NOT     *
*         Public Domain and remains the propery of ALGORITHM, Inc.,       *
*   Christopher D. Watkins and Stephen B. Coy.  This software may not be  *
*  reproduced or integrated into other packages without the prior written *
*          consent of Christopher D. Watkins and Stephen B. Coy.          *
*                                                                         *
*                       Requires: defs.h, extern.h                        *
*                                                                         *
***************************************************************************
*/

#include "Sphere_3D.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>
//#include <random>

#include "Bob.hpp"
#include "BobMath.hpp"
#include "Vector_3D.hpp"
#include "Isect_3D.hpp"
#include "Stats.hpp"
#include "defs.hpp"
#include "extern.hpp"


typedef struct t_spheredata {
    Vec sph_center;
    double sph_radius;  /* actually store 1/radius */
    double sph_fuzzy;   /* RAND_MAX/fuzzy part of radius */
    double sph_radius2; /* radius squared */
} SphereData;

Sphere_3D::Sphere_3D() : Object_3D() {
    o_type = T_SPHERE;
    o_surf = CurrentSurface;
}
Sphere_3D::~Sphere_3D() {
    Object_3D::~Object_3D();
    if (o_data != NULL) {
        delete (SphereData *)o_data;
    }
}

int Sphere_3D::intersect(Ray *ray, Isect &hit) {
    double b, disc, t, t0, t1, dot_vv;
    Point V, P;
    SphereData *sp;

    sp = (SphereData *)o_data;

    if (o_type == T_FUZZY) { /* its a fuzz ball */
        double newSphere;

        newSphere = 1.0 / sp->sph_radius + (double)rand() / sp->sph_fuzzy;
        sp->sph_radius2 = newSphere * newSphere;
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

    if (t0 > Bob::rayeps) { /* possible winner */
        if (!clips) {
            t = t0;
        } else {
            RayPoint(*ray, t0, P);
            if (clips->clip_check(P)) {
                t = t0;
            } else { /* well, try t1 */
                RayPoint(*ray, t1, P);
                if (clips->clip_check(P)) {
                    t = t1;
                } else {
                    return 0;
                }
            }
        }
    } else if (t1 > Bob::rayeps) {
        if (!clips) {
            t = t1;
        } else {
            RayPoint(*ray, t1, P);
            if (clips->clip_check(P)) {
                t = t1;
            } else {
                return 0;
            }
        }
    } else {
        return 0;
    }

    hit.isect_t = t;
    hit.isect_enter = dot_vv > sp->sph_radius2 + Bob::rayeps ? 1 : 0;
    hit.isect_self = hit.isect_enter ? NULL : this;
    hit.isect_prim = this;
    hit.isect_surf = o_surf;

    return 1;
}

void Sphere_3D::normal(Isect &hit, Point &P, Vec &N) {
    SphereData *sp;

    sp = (SphereData *)o_data;

    VecSub(P, sp->sph_center, N);
    VecNormalize(N);
}

Sphere_3D *Sphere_3D::makeSphere(Vec &pos, double radius, double fuzzy) {
    Sphere_3D *newSphere;
    int i;
    SphereData *sp;

    newSphere = new Sphere_3D();
    Stats::trackMemoryUsage(sizeof(Sphere_3D));
    Bob::getApp().parser.ptrchk(newSphere, "sphere object");
    if (fuzzy > Bob::rayeps) {
        newSphere->o_type = T_FUZZY;
    } else {
        newSphere->o_type = T_SPHERE;
    }

    if (Clip_3D::ClipTop) {
        newSphere->clips = Clip_3D::ClipTop;
        Clip_3D::ClipTop = GlobalClip::GlobalClipTop->clip;
    } else {
        newSphere->clips = NULL;
    }

    sp = new SphereData();
    Stats::trackMemoryUsage(sizeof(SphereData));
    Bob::getApp().parser.ptrchk(newSphere, "sphere data");
    VecCopy(pos, sp->sph_center);
    radius = bMath::abs(radius);
    if (radius < Bob::rayeps)
        radius = Bob::rayeps;
    sp->sph_radius = 1.0 / radius;
    sp->sph_radius2 = radius * radius;
    if (newSphere->o_type == T_FUZZY) {
        sp->sph_fuzzy = (double)RAND_MAX / fuzzy;
        radius += fuzzy; /* for calcing bounding slabs */
    }
    newSphere->o_data = (void *)sp;

    /*
     * figure out dmin and dmax values for
     * each of the slabs...
     */

    for (i = 0; i < NSLABS; i++) {
        newSphere->o_dmin[i] = VecDot(sp->sph_center, Slab[i]) - bMath::abs(radius);
        newSphere->o_dmax[i] = VecDot(sp->sph_center, Slab[i]) + bMath::abs(radius);
    }

    if (newSphere->clips) {
        Clip_3D::bound_opt(newSphere);
    }

    return newSphere;
}
