/*
***************************************************************************
*                                                                         *
*                             Bob Ray Tracer                              *
*                                                                         *
*                       Ring.C = the ring primative                       *
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

#include "Ring_3D.hpp"

#include <cmath>
#include <cstdio>

#include "Bob.hpp"
#include "BobMath.hpp"
#include "Vector_3D.hpp"
#include "Isect_3D.hpp"
#include "Clip_3D.hpp"
#include "Stats.hpp"
#include "defs.hpp"
#include "extern.hpp"


typedef struct t_ringdata {
    Vec ring_center;
    Vec ring_normal;
    double D;
    double min_radius, max_radius; /* actually store squared values */
} RingData;

Ring_3D::Ring_3D(): Object_3D() {
    Object_3D::o_type = T_RING;
    Object_3D::o_surf = Surface_3D::currentSurface;
}
Ring_3D::~Ring_3D() {
    Object_3D::~Object_3D();
    if (o_data != NULL) {
        delete (RingData*)o_data;
    }
}

int Ring_3D::intersect(Ray *ray, Isect &hit) {
    RingData *rp;
    double Vprd, Vpro, t, rad;
    Point point;

    rp = (RingData *)o_data; /* point to ring data */

    Vprd = VecDot(rp->ring_normal, ray->D);

    if (Vprd == 0) { /* ray in plane */
        return 0;
    }

    Vpro = -(VecDot(rp->ring_normal, ray->P) + rp->D);

    t = Vpro / Vprd;

    if (t < Bob::rayeps) { /* intersect behind us or self */
        return 0;
    }

    RayPoint(*ray, t, point);

    /* if clipping planes and doesn't pass, bail */
    if (clips && !clips->clip_check(point)) {
        return 0;
    }

    /* calc dist to ring center */
    VecSub(rp->ring_center, point, point);
    rad = VecDot(point, point); /* calc dist^2 */

    if (rad > rp->max_radius || rad < rp->min_radius) {
        return 0; /* missed the ring */
    }

    hit.isect_t = t;
    hit.isect_prim = this;
    hit.isect_self = this; /* rings are not self intersecting */
    hit.isect_surf = o_surf;

    return 1;
}

void Ring_3D::normal(Isect &hit, Point &P, Vec &N) {
    RingData *rp;
    rp = (RingData *)o_data;

    VecCopy(rp->ring_normal, N); /* already normalized */
}

Ring_3D *Ring_3D::makeRing(Vec &pos, Vec &norm, double min_rad, double max_rad) {
    Ring_3D *newRing;
    RingData *rp;

    newRing = new Ring_3D();
    Stats::trackMemoryUsage(sizeof(Ring_3D));


    if (Clip_3D::ClipTop) {
        newRing->clips = Clip_3D::ClipTop;
        Clip_3D::ClipTop = GlobalClip::GlobalClipTop->clip;
    } else {
        newRing->clips = NULL;
    }

    rp = new RingData();
    Stats::trackMemoryUsage(sizeof(RingData));

    VecCopy(pos, rp->ring_center);
    VecCopy(norm, rp->ring_normal);
    VecNormalize(rp->ring_normal);
    rp->min_radius = min_rad * min_rad;
    rp->max_radius = max_rad * max_rad;
    rp->D = -VecDot(rp->ring_center, rp->ring_normal);
    newRing->o_data = (void *)rp;

    /*
     * figure out dmin and dmax values for
     * each of the slabs...
     */

    max_rad = bMath::abs(max_rad); /* just to be sure... */
    for (int i = 0; i < NSLABS; i++) {
        double size = VecDot(Slab[i], rp->ring_normal);
        size = max_rad * sqrt(1.0 - size * size);
        newRing->o_dmin[i] = pos[i] - size;
        newRing->o_dmax[i] = pos[i] + size;
    }

    if (newRing->clips) {
        Clip_3D::bound_opt(newRing);
    }

    return newRing;
}
