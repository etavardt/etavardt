/*
***************************************************************************
*                                                                         *
*                             Bob Ray Tracer                              *
*                                                                         *
*         Trace.C = Where the rays are actually shot out into the         *
*        word with grand hopes of hitting a primitive and                 *
*        adding color to the scene.  Here we also deal with               *
*        those poor rays whose lot in life is to never hit                *
*        anything, proceding out into never-never land.                   *
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

    Trace returns the dist to the next hit.  This gives shade()
    a value to use for determining color attenuation for haze and
    transparent objects.
*/
#include "RayTrace_3D.hpp"

#include <cmath>
#include <cstdio>

#include "BobMath.hpp"
#include "Object_3D.hpp"
#include "Isect_3D.hpp"
#include "Color.hpp"

#include "extern.hpp"

unsigned long RayTrace_3D::nRays   = 0;
int           RayTrace_3D::deepest = 0; /* deepest level reached */

//class Isect;
//extern int  Intersect (Ray *ray , Isect &hit , double maxdist , Object *self);
//extern void Shade(int level, double weight, Point &P, Vec &N, Vec &I, Isect &hit, Color &col, double ior);
void RayTrace_3D::bkg(Vec &dir, Color &col) {
    if (background.color.r < 0.0) { // Using a color from a Pallet not a single color. Works how?
        double dot = -VecDot(dir, background.up);
        double index = 127.0 * dot + 128.0;
        int indx = index;
        index -= indx;
        col.r = (1.0 - index) * background.pal[indx].r / 256.0 + index * background.pal[indx + 1].r / 256.0;
        col.g = (1.0 - index) * background.pal[indx].g / 256.0 + index * background.pal[indx + 1].g / 256.0;
        col.b = (1.0 - index) * background.pal[indx].b / 256.0 + index * background.pal[indx + 1].b / 256.0;
    } else { // not using a Pallet
        //        VecCopy(background.color, col);
        //        col.r = background.color.r;
        //        col.g = background.color.g;
        //        col.b = background.color.b;
        col = background.color;
    }
} /* end of bkg */

//    double    ior;        /* current material ior */
double RayTrace_3D::trace(int level, double weight, Ray *ray, Color &color, double ior, Object *obj) {
    Point P;
    Vec N;
    Isect hit;

    if (level >= maxlevel) {
        //color.r = color.g = color.b = 0.0;
        color = 0.0;
        return 0.0;
    } else if (level > deepest) {
        deepest = level;
    }

    nRays++;

    if (RayTrace_3D::intersect(ray, hit, HUGE_NUM, obj)) {
        Object *prim = hit.isect_prim;
        RayPoint(*ray, hit.isect_t, P);
        /* get normal vector of intersection */
        prim->normal(hit, P, N);

        RayTrace_3D::shade(level, weight, P, N, ray->D, hit, color, ior);
        return hit.isect_t;
    } else {
        bkg(ray->D, color);

        return HUGE_NUM;
    }
}
