/*
***************************************************************************
*                                                                         *
*                             Bob Ray Tracer                              *
*                                                                         *
*                     Poly.C = the polygon primative                      *
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

#include "Poly_3D.hpp"

#include <cstdio>
#include <math.h>

#include "Bob.hpp"
#include "BobMath.hpp"
#include "Vector_3D.hpp"
#include "Point_3D.hpp"
#include "Isect_3D.hpp"
#include "Stats.hpp"
#include "Clip_3D.hpp"
#include "defs.hpp"
#include "extern.hpp"

#include "unused_macros.hpp"

typedef struct t_polydata {
    int poly_npoints;
    Vec *poly_point;
    Vec poly_normal;
    double poly_d;
    double poly_p1, poly_p2;
} PolyData;

Poly_3D::Poly_3D() : Object_3D() {
    Object_3D::o_type = T_POLY;
    Object_3D::o_surf = Surface_3D::currentSurface;
}
Poly_3D::~Poly_3D() {
    Object_3D::~Object_3D();
    if (o_data != NULL) {
        delete (PolyData*)o_data;
    }
}

int Poly_3D::intersect(Ray *ray, Isect &hit) {
    double n, d, t, m, b;
    Point V;
    int l = 0;
    int c1, c2;
    PolyData *pd;

    pd = (PolyData *)o_data;
    n = VecDot(ray->P, pd->poly_normal) + pd->poly_d;
    d = VecDot(ray->D, pd->poly_normal);

    /* check for ray in plane of polygon */

    if (bMath::abs(d) < Bob::rayeps) {
        return 0;
    }

    t = -n / d;
    if (t < Bob::rayeps) {
        return 0;
    }

    RayPoint(*ray, t, V);

    /* if clipping planes and doesn't pass, bail */
    if (clips && !clips->clip_check(V)) {
        return 0;
    }

    c1 = pd->poly_p1;
    c2 = pd->poly_p2;


    for (int i = 0; i < pd->poly_npoints; i++) {

        int j = (i + 1) % pd->poly_npoints;

        int qi = 0;
        int qj = 0;
        int ri = 0;
        int rj = 0;

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

        if ((-b / m) < Bob::rayeps)
            l++;
    }

    if ((l % 2) == 0)
        return 0;

    hit.isect_t = t;
    hit.isect_surf = o_surf;
    hit.isect_prim = this;
    hit.isect_self = this; /* polys are not self intersecting */

    return 1;
}

void Poly_3D::normal(Isect &UNUSED_VAR(hit), Point &UNUSED_VAR(P), Vec &N) {
    PolyData *pd = (PolyData *)o_data;
    VecCopy(pd->poly_normal, N);
}

Poly_3D *Poly_3D::makePoly(int npoints, Vec *points) {
    Poly_3D *newPoly;
    PolyData *pd;
    Vec P1, P2;

    newPoly = new Poly_3D();
    Stats::trackMemoryUsage(sizeof(Poly_3D));


    if (Clip_3D::ClipTop) {
        newPoly->clips = Clip_3D::ClipTop;
        Clip_3D::ClipTop = GlobalClip::GlobalClipTop->clip;
    } else {
        newPoly->clips = NULL;
    }

    pd = new PolyData();
    Stats::trackMemoryUsage(sizeof(PolyData));

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

    newPoly->o_data = (void *)pd;

    /*
     * now, calculate the values of
     * the dmin and dmax 'es for the globally defined slabs...
     */

    for (int i = 0; i < NSLABS; i++) {
        double dmin = HUGE_NUM;
        double dmax = -HUGE_NUM;

        for (int j = 0; j < pd->poly_npoints; j++) {
            double d = VecDot(Slab[i], pd->poly_point[j]);
            if (d < dmin)
                dmin = d;
            if (d > dmax)
                dmax = d;
        }
        newPoly->o_dmin[i] = dmin;
        newPoly->o_dmax[i] = dmax;
    }

    if (newPoly->clips) {
        Clip_3D::bound_opt(newPoly);
    }

    return newPoly;
}
