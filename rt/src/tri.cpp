/*
�������������������������������������������
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�                    Tri.C = the triangular patch primitive               �
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

    TRIs are triangular patches with normals defined at the vertices.
    When an intersection is found, it interpolates the normal to the
    surface at that point.

    Algorithm is due to Jeff Arenburg, (arenburg@trwrb.uucp) and was
    posted to USENET.

    Basically, for each triangle we calculate an inverse transformation
    matrix, and use it to determine the point of intersection in the plane
    of the triangle relative to the "base point" of the triangle.  We then
    figure its coordinates relative to that base point.  These base points
    are used to find the barycentric coordinates, and then in normal
    interpolation...
*/

#include "Bob.hpp"
#include "Stats.hpp"
#include "Tri_3D.hpp"
#include "defs.hpp"
#include "extern.hpp"
#include "proto.hpp"
#include <cstdio>
#include <math.h>

/*
ObjectProcs TriProcs = {
    TriIntersect,
    TriNormal,
};
*/

typedef struct t_patchdata {
    Vec tri_P[3];
    Vec tri_N[3];
    Vec tri_bb[3];
    Vec normal; /* place to store normal */
} TriData;

Tri_3D::Tri_3D() : Object_3D() {
    Object_3D::o_type = T_TRI;
    Object_3D::o_surf = CurrentSurface;
}
Tri_3D::~Tri_3D() {
    Object_3D::~Object_3D();
    if (o_data != NULL) {
        delete (TriData *)o_data;
    }
}
int Tri_3D::intersect(Object *obj, Ray *ray, Isect *hit) {
    TriData *td;
    Flt n, d, dist;
    Flt r, s, t;
    Flt a, b;
    Vec Q;

    td = (TriData *)obj->o_data;

    /*
     * The matrix td->tri_bb transforms vectors in the world
     * space into a space with the following properties.
     *
     * 1.  The sides of the triangle are coincident with the
     *     x and y axis, and have unit length.
     * 2.  The normal to the triangle is coincident with the
     *     z axis.
     *
     */

    /*
     * d is the slope with respect to the z axis.  If d is zero, then
     * the ray is parallel to the plane of the polygon, and we count
     * it as a miss...
     */

    d = VecDot(ray->D, td->tri_bb[2]);
    if (ABS(d) < rayeps)
        return 0;

    /*
     * Q is a vector from the eye to the triangles "origin" vertex.
     * n is then set to be the distance of the tranformed eyepoint
     * to the plane in the polygon.
     * Together, n and d allow you to find the distance to the polygon,
     * which is merely n / d.
     */

    VecSub(td->tri_P[0], ray->P, Q);

    n = VecDot(Q, td->tri_bb[2]);

    dist = n / d;

    if (dist < rayeps) {
        return 0;
    }

    /* calc intersect point, Q */
    RayPoint(ray, dist, Q);

    /* if clipping and doesn't pass, bail */
    if (obj->clips && !clip_check(obj->clips, Q)) {
        return 0;
    }

    /*
     * Q is the point we hit.  Find its position relative to the
     * origin of the triangle.
     */

    VecSub(Q, td->tri_P[0], Q);

    a = VecDot(Q, td->tri_bb[0]);
    b = VecDot(Q, td->tri_bb[1]);

    if (a < 0.0 || b < 0.0 || a + b > 1.0) {
        return 0;
    }

    r = 1.0 - a - b;
    s = a;
    t = b;

    hit->isect_t = dist;
    hit->isect_prim = obj;
    hit->isect_self = obj;
    hit->isect_surf = obj->o_surf;

    /* sum baricentric components to get real normal */

    VecZero(td->normal);
    VecAddS(r, td->tri_N[0], td->normal, td->normal);
    VecAddS(s, td->tri_N[1], td->normal, td->normal);
    VecAddS(t, td->tri_N[2], td->normal, td->normal);

    return 1;
}

void Tri_3D::normal(Object *obj, Isect *hit, Point P, Point N) {
    TriData *td;

    td = (TriData *)obj->o_data;

    VecNormalize(td->normal);
    VecCopy(td->normal, N);
}

Tri_3D *Tri_3D::makeTri(Vec *point) {
    Tri_3D *o;
    TriData *td;
    int i, j;
    Flt dmin, dmax, d;
    Vec B[3];

    checkTri(point);

    o = new Tri_3D();
    Stats::trackMemoryUsage(sizeof(Tri_3D));
    Bob::getApp().parser.ptrchk(o, "patch object");
//    o->o_type = T_TRI;
//    o->o_procs = &TriProcs;
//    o->o_surf = CurrentSurface;

    td = new TriData();
    Stats::trackMemoryUsage(sizeof(TriData));
    Bob::getApp().parser.ptrchk(td, "patch data");

    if (ClipTop) {
        o->clips = ClipTop;
        ClipTop = GlobalClipTop->clip;
    } else {
        o->clips = NULL;
    }

    /*
     * copy in the points....
     */
    VecCopy(point[0], td->tri_P[0]);
    VecCopy(point[2], td->tri_P[1]);
    VecCopy(point[4], td->tri_P[2]);

    /*
     * and the normals, then normalize them...
     */
    VecCopy(point[1], td->tri_N[0]);
    VecCopy(point[3], td->tri_N[1]);
    VecCopy(point[5], td->tri_N[2]);
    VecNormalize(td->tri_N[0]);
    VecNormalize(td->tri_N[1]);
    VecNormalize(td->tri_N[2]);

    /*
     * construct the inverse of the matrix...
     * | P1 |
     * | P2 |
     * | N  |
     * and store it in td->tri_bb[]
     */

    VecSub(td->tri_P[1], td->tri_P[0], B[0]);
    VecSub(td->tri_P[2], td->tri_P[0], B[1]);
    VecCross(B[0], B[1], B[2]);
    VecNormalize(B[2]);

    invertMatrix(B, td->tri_bb);

    for (i = 0; i < NSLABS; i++) {
        dmin = HUGE;
        dmax = -HUGE;

        for (j = 0; j < 3; j++) {
            d = VecDot(Slab[i], td->tri_P[j]);
            if (d < dmin)
                dmin = d;
            if (d > dmax)
                dmax = d;
        }
        o->o_dmin[i] = dmin - rayeps;
        o->o_dmax[i] = dmax + rayeps;
    }

    o->o_data = (void *)td;

    return o;
}

void Tri_3D::invertMatrix(Vec in[3], Vec out[3]) {
    int i, j;
    Flt det;

    out[0][0] = (in[1][1] * in[2][2] - in[1][2] * in[2][1]);
    out[1][0] = -(in[0][1] * in[2][2] - in[0][2] * in[2][1]);
    out[2][0] = (in[0][1] * in[1][2] - in[0][2] * in[1][1]);

    out[0][1] = -(in[1][0] * in[2][2] - in[1][2] * in[2][0]);
    out[1][1] = (in[0][0] * in[2][2] - in[0][2] * in[2][0]);
    out[2][1] = -(in[0][0] * in[1][2] - in[0][2] * in[1][0]);

    out[0][2] = (in[1][0] * in[2][1] - in[1][1] * in[2][0]);
    out[1][2] = -(in[0][0] * in[2][1] - in[0][1] * in[2][0]);
    out[2][2] = (in[0][0] * in[1][1] - in[0][1] * in[1][0]);

    det = in[0][0] * in[1][1] * in[2][2] + in[0][1] * in[1][2] * in[2][0] + in[0][2] * in[1][0] * in[2][1] - in[0][2] * in[1][1] * in[2][0] - in[0][0] * in[1][2] * in[2][1] - in[0][1] * in[1][0] * in[2][2];

    det = 1 / det;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            out[i][j] *= det;
        }
    }
}

/*
    CheckTri() -- make sure that all the vertex normals are on the
        same side of the patch.
*/

void Tri_3D::checkTri(Vec *point) {
    Vec N, A, B;
    int i;
    Flt dot;

    /* calc surface normal as cross of edge vectors */

    VecSub(point[0], point[2], A);
    VecSub(point[0], point[4], B);
    VecCross(A, B, N);

    /* compare with each normal and flip if needed to same side */

    for (i = 0; i < 3; i++) {
        dot = VecDot(N, point[i * 2 + 1]);
        if (dot <= 0.0) {
            VecNegate(point[i * 2 + 1]);
        }
    }
}
