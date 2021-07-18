/*
�������������������������������������������
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�                     Poly.C = the polygon primative                      �
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

#include "Poly_3D.hpp"

#include <cstdio>
#include <math.h>

#include "Bob.hpp"
#include "BobMath.hpp"
#include "Vector_3D.hpp"
#include "Point_3D.hpp"

#include "Isect_3D.hpp"
#include "Stats.hpp"
#include "defs.hpp"
#include "extern.hpp"
#include "proto.hpp"

typedef struct t_polydata {
    int poly_npoints;
    Vec *poly_point;
    Vec poly_normal;
    double poly_d;
    double poly_p1, poly_p2;
} PolyData;

Poly_3D::Poly_3D() : Object_3D() {
    Object_3D::o_type = T_POLY;
    Object_3D::o_surf = CurrentSurface;
}
Poly_3D::~Poly_3D() {
    Object_3D::~Object_3D();
    if (o_data != NULL) {
        delete (PolyData*)o_data;
    }
}

int Poly_3D::intersect(Object *obj, Ray *ray, Isect &hit) {
    double n, d, t, m, b;
    Point V;
    int i, j, l;
    int qi, qj;
    int ri, rj;
    int c1, c2;
    PolyData *pd;

    pd = (PolyData *)obj->o_data;
    n = VecDot(ray->P, pd->poly_normal) + pd->poly_d;
    d = VecDot(ray->D, pd->poly_normal);

    /* check for ray in plane of polygon */

    if (bMath::abs(d) < rayeps) {
        return 0;
    }

    t = -n / d;
    if (t < rayeps) {
        return 0;
    }

    RayPoint(ray, t, V);

    /* if clipping planes and doesn't pass, bail */
    if (obj->clips && !clip_check(obj->clips, V)) {
        return 0;
    }

    c1 = pd->poly_p1;
    c2 = pd->poly_p2;

    l = 0;
    for (i = 0; i < pd->poly_npoints; i++) {

        j = (i + 1) % pd->poly_npoints;

        qi = 0;
        qj = 0;
        ri = 0;
        rj = 0;

        if (pd->poly_point[i][c2] == pd->poly_point[j][c2])
            continue; /*ignore horizontal lines */

        if (pd->poly_point[i][c2] < V[c2])
            qi = 1;
        if (pd->poly_point[j][c2] < V[c2])
            qj = 1;
        if (qi == qj)
            continue;

        if (pd->poly_point[i][c1] < V[c1])
            ri = 1;
        if (pd->poly_point[j][c1] < V[c1])
            rj = 1;

        if (ri & rj) {
            l++;
            continue;
        }

        if ((ri | rj) == 0)
            continue;

        /*
         * more difficult acceptance...
         */
        m = (pd->poly_point[j][c2] - pd->poly_point[i][c2]) / (pd->poly_point[j][c1] - pd->poly_point[i][c1]);

        b = (pd->poly_point[j][c2] - V[c2]) - m * (pd->poly_point[j][c1] - V[c1]);

        if ((-b / m) < rayeps)
            l++;
    }

    if ((l % 2) == 0)
        return 0;

    hit.isect_t = t;
    hit.isect_surf = obj->o_surf;
    hit.isect_prim = obj;
    hit.isect_self = obj; /* polys are not self intersecting */

    return 1;
}

void Poly_3D::normal(Object_3D *obj, Isect &hit, Point &P, Vec &N) {
    PolyData *pd;
    pd = (PolyData *)obj->o_data;
    VecCopy(pd->poly_normal, N);
}

Poly_3D *Poly_3D::makePoly(int npoints, Vec *points) {
    Poly_3D *obj;
    PolyData *pd;
    Vec P1, P2;
    double d, dmax, dmin;
    int i, j;

    obj = new Poly_3D();
    Stats::trackMemoryUsage(sizeof(Poly_3D));
    Bob::getApp().parser.ptrchk(obj, "polygon object");

    if (ClipTop) {
        obj->clips = ClipTop;
        ClipTop = GlobalClipTop->clip;
    } else {
        obj->clips = NULL;
    }

    pd = new PolyData();
    Stats::trackMemoryUsage(sizeof(PolyData));
    Bob::getApp().parser.ptrchk(pd, "polygon data");
    pd->poly_npoints = npoints;
    pd->poly_point = points;

    /*
     * calculate the normal by giving various cross products...
     */

    VecSub(pd->poly_point[0], pd->poly_point[1], P1);
    VecSub(pd->poly_point[2], pd->poly_point[1], P2);

    VecCross(P1, P2, pd->poly_normal);
    VecNormalize(pd->poly_normal);

    if (bMath::abs(pd->poly_normal[0]) >= bMath::abs(pd->poly_normal[1]) && bMath::abs(pd->poly_normal[0]) >= bMath::abs(pd->poly_normal[2])) {
        pd->poly_p1 = 1;
        pd->poly_p2 = 2;
    } else if (bMath::abs(pd->poly_normal[1]) >= bMath::abs(pd->poly_normal[0]) && bMath::abs(pd->poly_normal[1]) >= bMath::abs(pd->poly_normal[2])) {
        pd->poly_p1 = 0;
        pd->poly_p2 = 2;
    } else {
        pd->poly_p1 = 0;
        pd->poly_p2 = 1;
    }

    pd->poly_d = -VecDot(pd->poly_normal, pd->poly_point[0]);

    obj->o_data = (void *)pd;

    /*
     * now, calculate the values of
     * the dmin and dmax 'es for the globally defined slabs...
     */

    for (i = 0; i < NSLABS; i++) {
        dmin = HUGE_NUM;
        dmax = -HUGE_NUM;

        for (j = 0; j < pd->poly_npoints; j++) {
            d = VecDot(Slab[i], pd->poly_point[j]);
            if (d < dmin)
                dmin = d;
            if (d > dmax)
                dmax = d;
        }
        obj->o_dmin[i] = dmin;
        obj->o_dmax[i] = dmax;
    }

    if (obj->clips) {
        bound_opt(obj);
    }

    return obj;
}
