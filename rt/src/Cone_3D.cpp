/*
***************************************************************************
*                                                                         *
*                             Bob Ray Tracer                              *
*                                                                         *
*                       Cone.C = newCone primative                        *
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

#include "Cone_3D.hpp"

#include <cstdio>
#include <math.h>

#include "Clip_3D.hpp"
#include "Bob.hpp"
#include "BobMath.hpp"
#include "Vector_3D.hpp"
#include "Point_3D.hpp"
#include "Isect_3D.hpp"
#include "Stats.hpp"
#include "defs.hpp"
#include "extern.hpp"


typedef struct t_conedata {
    Vec cone_base;
    double cone_base_radius;
    double cone_base_d;
    Vec cone_apex;
    double cone_apex_radius;
    Vec cone_u;
    Vec cone_v;
    Vec cone_w; /* vector along newCone axis */
    double cone_height;
    double cone_slope;
    double cone_min_d;
    double cone_max_d;
} ConeData;

Cone_3D::Cone_3D() : Object_3D() {
    Object_3D::o_type = T_CONE;
    Object_3D::o_surf = Surface_3D::currentSurface;
}
Cone_3D::~Cone_3D() {
    Object_3D::~Object_3D();
    if (o_data != NULL) {
        delete (ConeData *)o_data;
    }
}

int Cone_3D::intersect(Ray *ray, Isect &hit) {
    Ray tray;
    ConeData *cd;
    Vec V, P;
    double a, b, c, d;
    double t1, t2;

    cd = (ConeData *)(o_data);

    /*
     * First, we get the coordinates of the ray origin in
     * the objects space....
     */

    VecSub(ray->P, cd->cone_base, V);

    tray.P[0] = VecDot(V, cd->cone_u);
    tray.P[1] = VecDot(V, cd->cone_v);
    tray.P[2] = VecDot(V, cd->cone_w);

    /*
    VecAdd(ray->P, ray->D, V);
    VecSub(V, cd->cone_base, V);
    */

    tray.D[0] = VecDot(ray->D, cd->cone_u);
    tray.D[1] = VecDot(ray->D, cd->cone_v);
    tray.D[2] = VecDot(ray->D, cd->cone_w);

    /*
    VecSub(tray.D, tray.P, tray.D);
    */

    a = tray.D[0] * tray.D[0] + tray.D[1] * tray.D[1] - cd->cone_slope * cd->cone_slope * tray.D[2] * tray.D[2];

    b = 2.0 * (tray.P[0] * tray.D[0] + tray.P[1] * tray.D[1] - cd->cone_slope * cd->cone_slope * tray.P[2] * tray.D[2] - cd->cone_base_radius * cd->cone_slope * tray.D[2]);

    c = cd->cone_slope * tray.P[2] + cd->cone_base_radius;
    c = tray.P[0] * tray.P[0] + tray.P[1] * tray.P[1] - (c * c);

    if (a == 0) { /* special case? */
        t2 = -c / b;
        if (t2 < Bob::rayeps)
            return 0;
        t1 = -1.0;
    } else {
        double disc = b * b - 4.0 * a * c;
        if (disc < 0.0) {
            return 0;
        }

        disc = (double)sqrt(disc);
        t1 = (-b - disc) / (2.0 * a);
        t2 = (-b + disc) / (2.0 * a);

        if (t1 < 0.0 && t2 < 0.0)
            return 0;

        if (t2 < t1) { /* flop to look at t1 first */
            double flt_tmp = t1;
            t1 = t2;
            t2 = flt_tmp;
        }
    }

    /* check nearest root first */

    if (t1 > Bob::rayeps) { /* possible real hit */
        RayPoint(*ray, t1, P);
        if (!clips ||  clips->clip_check(P)) {
            d = VecDot(cd->cone_w, P);
            if (d >= cd->cone_min_d && d <= cd->cone_max_d) { /* hit! */
                hit.isect_t = t1;
                hit.isect_prim = this;
                hit.isect_surf = o_surf;

                return 1;
            }
        }
    }

    if (t2 > Bob::rayeps) {
        RayPoint(*ray, t2, P);
        if (!clips || clips->clip_check(P)) {
            d = VecDot(cd->cone_w, P);
            if (d >= cd->cone_min_d && d <= cd->cone_max_d) {
                hit.isect_t = t2;
                hit.isect_prim = this;
                hit.isect_surf = o_surf;

                return 1;
            }
        }
    }
    return 0;
}

void Cone_3D::normal(Isect &hit, Point &P, Vec &N) {
    double t;
    Vec V;
    ConeData *cd;

    cd = (ConeData *)o_data;

    /*
     * fill in the real normal...
     * Project the point onto the base plane.  The normal is
     * a vector from the basepoint through this point, plus the slope
     * times the cone_w vector...
     */
    //Point P = _P;
    t = -(VecDot(P, cd->cone_w) + cd->cone_base_d);
    VecAddS(t, cd->cone_w, P, V);
    VecSub(V, cd->cone_base, N);
    VecNormalize(N);
    VecAddS(-cd->cone_slope, cd->cone_w, N, N);
    VecNormalize(N);

    hit.isect_self = NULL;
}

Cone_3D *Cone_3D::makeCone(Vec &basepoint, double baseradius, Vec &apexpoint, double apexradius) {
    Cone_3D *newCone;
    ConeData *cd;
    double ftmp = 0.0;
//    double dmin, dmax, ftmp, size;
    Vec tmp;
//    int i;

    newCone = new Cone_3D();
    Stats::trackMemoryUsage(sizeof(Cone_3D));


    if (Clip_3D::ClipTop) {
        newCone->clips = Clip_3D::ClipTop;
        Clip_3D::ClipTop = GlobalClip::GlobalClipTop->clip;
    } else {
        newCone->clips = NULL;
    }

    cd = new ConeData();
    Stats::trackMemoryUsage(sizeof(ConeData));


    VecCopy(basepoint, cd->cone_base);
    VecCopy(apexpoint, cd->cone_apex);

    baseradius = bMath::abs(baseradius);
    apexradius = bMath::abs(apexradius);
    cd->cone_base_radius = baseradius;
    cd->cone_apex_radius = apexradius;

    VecSub(apexpoint, basepoint, cd->cone_w);
    cd->cone_height = VecNormalize(cd->cone_w);
    cd->cone_slope = (cd->cone_apex_radius - cd->cone_base_radius) / (cd->cone_height);
    cd->cone_base_d = -VecDot(basepoint, cd->cone_w);

    MakeVector(0.0, 0.0, 1.0, tmp);
    ftmp = VecDot(tmp, cd->cone_w);
    ftmp = bMath::abs(ftmp);
    ftmp = bMath::abs(ftmp - 1.0);
    if (ftmp < Bob::rayeps) {
        MakeVector(0.0, 1.0, 0.0, tmp);
    }

    /* find two axes which are at right angles to cone_w
     */

    VecCross(cd->cone_w, tmp, cd->cone_u);
    VecCross(cd->cone_u, cd->cone_w, cd->cone_v);

    VecNormalize(cd->cone_u);
    VecNormalize(cd->cone_v);

    cd->cone_min_d = VecDot(cd->cone_w, cd->cone_base);
    cd->cone_max_d = VecDot(cd->cone_w, cd->cone_apex);

    if (cd->cone_max_d < cd->cone_min_d) {
        ftmp = cd->cone_max_d;
        cd->cone_max_d = cd->cone_min_d;
        cd->cone_min_d = ftmp;
    }

    newCone->o_data = (void *)cd;

    /* calc bounding slabs */

    VecCopy(cd->cone_w, tmp); /* get vector along axis */

    for (int i = 0; i < NSLABS; i++) {
        double dmin = 0.0, dmax = 0.0;
        double size = VecDot(Slab[i], tmp);
        size = baseradius * sqrt(1.0 - size * size);
        newCone->o_dmin[i] = basepoint[i] - size;
        newCone->o_dmax[i] = basepoint[i] + size;
        size = VecDot(Slab[i], tmp);
        size = apexradius * sqrt(1.0 - size * size);
        dmin = apexpoint[i] - size;
        dmax = apexpoint[i] + size;
        if (dmin < newCone->o_dmin[i])
            newCone->o_dmin[i] = dmin;
        if (dmax > newCone->o_dmax[i])
            newCone->o_dmax[i] = dmax;
    }

    if (newCone->clips) {
        Clip_3D::bound_opt(newCone);
    }

    return newCone;
}
