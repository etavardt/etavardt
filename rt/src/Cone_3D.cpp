/*
�������������������������������������������
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�                       Cone.C = cone primative                           �
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

#include "Cone_3D.hpp"
#include "Bob.hpp"
#include "Isect_3D.hpp"
#include "Stats.hpp"
#include "defs.hpp"
#include "extern.hpp"
#include <cstdio>
#include <math.h>

#include "proto.hpp"

typedef struct t_conedata {
    Vec cone_base;
    Flt cone_base_radius;
    Flt cone_base_d;
    Vec cone_apex;
    Flt cone_apex_radius;
    Vec cone_u;
    Vec cone_v;
    Vec cone_w; /* vector along cone axis */
    Flt cone_height;
    Flt cone_slope;
    Flt cone_min_d;
    Flt cone_max_d;
} ConeData;
/*
ObjectProcs ConeProcs = {
    ConeIntersect,
    ConeNormal,
};
*/
Cone_3D::Cone_3D() : Object_3D() {
    Object_3D::o_type = T_CONE;
    Object_3D::o_surf = CurrentSurface;
}
Cone_3D::~Cone_3D() {
    Object_3D::~Object_3D();
    if (o_data != NULL) {
        delete (ConeData *)o_data;
    }
}

int Cone_3D::intersect(Object *obj, Ray *ray, Isect &hit) {
    Ray tray;
    ConeData *cd;
    Vec V, P;
    Flt a, b, c, d, disc;
    Flt t1, t2, flt_tmp;

    cd = (ConeData *)(obj->o_data);

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
        if (t2 < rayeps)
            return 0;
        t1 = -1.0;
    } else {
        disc = b * b - 4.0 * a * c;
        if (disc < 0.0) {
            return 0;
        }

        disc = (Flt)sqrt(disc);
        t1 = (-b - disc) / (2.0 * a);
        t2 = (-b + disc) / (2.0 * a);

        if (t1 < 0.0 && t2 < 0.0)
            return 0;

        if (t2 < t1) { /* flop to look at t1 first */
            flt_tmp = t1;
            t1 = t2;
            t2 = flt_tmp;
        }
    }

    /* check nearest root first */

    if (t1 > rayeps) { /* possible real hit */
        RayPoint(ray, t1, P);
        if (!obj->clips || clip_check(obj->clips, P)) {
            d = VecDot(cd->cone_w, P);
            if (d >= cd->cone_min_d && d <= cd->cone_max_d) { /* hit! */
                hit.isect_t = t1;
                hit.isect_prim = obj;
                hit.isect_surf = obj->o_surf;

                return 1;
            }
        }
    }

    if (t2 > rayeps) {
        RayPoint(ray, t2, P);
        if (!obj->clips || clip_check(obj->clips, P)) {
            d = VecDot(cd->cone_w, P);
            if (d >= cd->cone_min_d && d <= cd->cone_max_d) {
                hit.isect_t = t2;
                hit.isect_prim = obj;
                hit.isect_surf = obj->o_surf;

                return 1;
            }
        }
    }
    return 0;
}

void Cone_3D::normal(Object *obj, Isect &hit, Point P, Point N) {
    Flt t;
    Vec V;
    ConeData *cd;

    cd = (ConeData *)obj->o_data;

    /*
     * fill in the real normal...
     * Project the point onto the base plane.  The normal is
     * a vector from the basepoint through this point, plus the slope
     * times the cone_w vector...
     */

    t = -(VecDot(P, cd->cone_w) + cd->cone_base_d);
    VecAddS(t, cd->cone_w, P, V);
    VecSub(V, cd->cone_base, N);
    VecNormalize(N);
    VecAddS(-cd->cone_slope, cd->cone_w, N, N);
    VecNormalize(N);

    hit.isect_self = NULL;
}

Cone_3D *Cone_3D::makeCone(Vec basepoint, Flt baseradius, Vec apexpoint, Flt apexradius) {
    Cone_3D *obj;
    ConeData *cd;
    Flt dmin, dmax, ftmp, size;
    Vec tmp;
    int i;

    obj = new Cone_3D();
    Stats::trackMemoryUsage(sizeof(Cone_3D));
    Bob::getApp().parser.ptrchk(obj, "cone object");
    //    obj->o_type = T_CONE;
    //    obj->o_procs = &ConeProcs;
    //    obj->o_surf = CurrentSurface;

    if (ClipTop) {
        obj->clips = ClipTop;
        ClipTop = GlobalClipTop->clip;
    } else {
        obj->clips = NULL;
    }

    cd = new ConeData();
    Stats::trackMemoryUsage(sizeof(ConeData));
    Bob::getApp().parser.ptrchk(cd, "cone data");

    VecCopy(basepoint, cd->cone_base);
    VecCopy(apexpoint, cd->cone_apex);

    baseradius = ABS(baseradius);
    apexradius = ABS(apexradius);
    cd->cone_base_radius = baseradius;
    cd->cone_apex_radius = apexradius;

    VecSub(apexpoint, basepoint, cd->cone_w);
    cd->cone_height = VecNormalize(cd->cone_w);
    cd->cone_slope = (cd->cone_apex_radius - cd->cone_base_radius) / (cd->cone_height);
    cd->cone_base_d = -VecDot(basepoint, cd->cone_w);

    MakeVector(0.0, 0.0, 1.0, tmp);
    ftmp = VecDot(tmp, cd->cone_w);
    ftmp = ABS(ftmp);
    ftmp = ABS(ftmp - 1.0);
    if (ftmp < rayeps) {
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

    obj->o_data = (void *)cd;

    /* calc bounding slabs */

    VecCopy(cd->cone_w, tmp); /* get vector along axis */

    for (i = 0; i < NSLABS; i++) {
        size = VecDot(Slab[i], tmp);
        size = baseradius * sqrt(1.0 - size * size);
        obj->o_dmin[i] = basepoint[i] - size;
        obj->o_dmax[i] = basepoint[i] + size;
        size = VecDot(Slab[i], tmp);
        size = apexradius * sqrt(1.0 - size * size);
        dmin = apexpoint[i] - size;
        dmax = apexpoint[i] + size;
        if (dmin < obj->o_dmin[i])
            obj->o_dmin[i] = dmin;
        if (dmax > obj->o_dmax[i])
            obj->o_dmax[i] = dmax;
    }

    if (obj->clips) {
        bound_opt(obj);
    }

    return obj;
}
