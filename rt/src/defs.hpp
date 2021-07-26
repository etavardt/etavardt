#pragma once
/*
***************************************************************************
*                                                                         *
*                             Bob Ray Tracer                              *
*                                                                         *
*                    Defs.C = the center of all confusion                 *
*                                                                         *
*       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     *
*                                                                         *
*       ALL RIGHTS RESERVED.   This software is published, but is NOT     *
*         Public Domain and remains the propery of ALGORITHM, Inc.,       *
*   Christopher D. Watkins and Stephen B. Coy.  This software may not be  *
*  reproduced or integrated into other packages without the prior written *
*          consent of Christopher D. Watkins and Stephen B. Coy.          *
*                                                                         *
*                          Requires: config.h                             *
*                                                                         *
***************************************************************************
*/
//#ifndef DEFS_HPP
//#define DEFS_HPP

#include "config.hpp"

/*
    typedefs for the world
*/

//typedef double Flt;
//typedef Flt Vec[3];
//typedef Vec Point;

// typedef double Matrix[4][4];
/*----------------------------------------------------------------------*/
/*
#ifndef DUMB_CPP

#define MakeVector(x, y, z, v) (v)[0] = (x), (v)[1] = (y), (v)[2] = (z)
#define VecNegate(a)    \
    (a)[0] = (-(a)[0]); \
    (a)[1] = (-(a)[1]); \
    (a)[2] = (-(a)[2]);
#define VecDot(a, b) ((a)[0] * (b)[0] + (a)[1] * (b)[1] + (a)[2] * (b)[2])
#define VecLen(a) (sqrt(VecDot(a, a)))
#define VecCopy(a, b) \
    (b)[0] = (a)[0];  \
    (b)[1] = (a)[1];  \
    (b)[2] = (a)[2];
#define VecAdd(a, b, c)       \
    (c)[0] = (a)[0] + (b)[0]; \
    (c)[1] = (a)[1] + (b)[1]; \
    (c)[2] = (a)[2] + (b)[2]
#define VecSub(a, b, c)       \
    (c)[0] = (a)[0] - (b)[0]; \
    (c)[1] = (a)[1] - (b)[1]; \
    (c)[2] = (a)[2] - (b)[2]
#define VecComb(A, a, B, b, c)            \
    (c)[0] = (A) * (a)[0] + (B) * (b)[0]; \
    (c)[1] = (A) * (a)[1] + (B) * (b)[1]; \
    (c)[2] = (A) * (a)[2] + (B) * (b)[2]
#define VecS(A, a, b)      \
    (b)[0] = (A) * (a)[0]; \
    (b)[1] = (A) * (a)[1]; \
    (b)[2] = (A) * (a)[2]
#define VecAddS(A, a, b, c)         \
    (c)[0] = (A) * (a)[0] + (b)[0]; \
    (c)[1] = (A) * (a)[1] + (b)[1]; \
    (c)[2] = (A) * (a)[2] + (b)[2]
#define VecMul(a, b, c)       \
    (c)[0] = (a)[0] * (b)[0]; \
    (c)[1] = (a)[1] * (b)[1]; \
    (c)[2] = (a)[2] * (b)[2]
#define VecCross(a, b, c)                       \
    (c)[0] = (a)[1] * (b)[2] - (a)[2] * (b)[1]; \
    (c)[1] = (a)[2] * (b)[0] - (a)[0] * (b)[2]; \
    (c)[2] = (a)[0] * (b)[1] - (a)[1] * (b)[0]
#define VecZero(v) \
    (v)[0] = 0.0;  \
    (v)[1] = 0.0;  \
    v[2] = 0.0
#define VecPrint(msg, v) fprintf(stderr, "%s: %g %g %g\n", msg, \
                                 (v)[0], (v)[1], (v)[2])

#endif /* not DUMB_CPP * /
 */

/* surface flags */

#define S_CACHE (0x0001)
#define S_NO_ANTIALIAS (0x0002)
#define S_TRANSFORM (0x0004)

#define S_TM_DIFF (0x0008)
#define S_TM_SPEC (0x0010)
#define S_TM_TRANS (0x0020)
#define S_TM_AMB (0x0040)
#define S_TM_MAPPING (0x0078) /* all bits for mapping */

/*    light type constants */

#define L_DIRECTIONAL (1)
#define L_POINT (2)
#define L_SPHERICAL (3)
#define L_SPOT (4)

/*      light illumination constants */

#define L_INFINITE (0)
#define L_R (1)
#define L_R_SQUARED (2)

/*      misc light flags */

#define L_NOSHADOWS (1)
#define L_NOSPEC (2)

/*
    Clipping
*/

#define C_PLANE (0x01)
#define C_SPHERE (0x02)
#define C_CONE (0x04)
#define C_INSIDE (0x10)

/* primitive types */

#define T_COMPOSITE (0)
#define T_SPHERE (1)
#define T_POLY (2)
#define T_CONE (3)
#define T_RING (4)
#define T_TRI (5)
#define T_FUZZY (6)

/* camera projections types */

#define P_FLAT (0)
#define P_ORTHOGRAPHIC (1)
#define P_FISHEYE (2)
#define P_NO_PARALLAX (3)

/* antialiasing mode */

#define A_NONE (0)
#define A_CORNERS (1)
#define A_QUICK (2)
#define A_ADAPTIVE (3)

//#endif //DEFS_HPP
