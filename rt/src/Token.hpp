#pragma once
/*
***************************************************************************
*                                                                         *
*                             Bob Ray Tracer                              *
*                                                                         *
*          Tokens.H = contains the token values used by the parser        *
*               as returned by the lexer.                                 *
*                                                                         *
*       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     *
*                                                                         *
*       ALL RIGHTS RESERVED.   This software is published, but is NOT     *
*         Public Domain and remains the propery of ALGORITHM, Inc.,       *
*   Christopher D. Watkins and Stephen B. Coy.  This software may not be  *
*  reproduced or integrated into other packages without the prior written *
*          consent of Christopher D. Watkins and Stephen B. Coy.          *
*                                                                         *
***************************************************************************
*/
#include "String.hpp"

enum TokenType {

/* misc stuff */

    END_OF_FILE     = -1,
    UNKNOWN         = 0,
    NEWFILE         = 1,
    POPFILE         = 2,
    NUMBER          = 3,
    COMMENT         = 4,

/* punctuation */

    LEFT_BRACE      = 10,
    RIGHT_BRACE     = 11,

/* studio */

    STUDIO          = 20,
    FROM            = 21,
    AT              = 22,
    UP              = 23,
    ANGLE           = 24,
    RESOLUTION      = 25,
    BKG             = 26,
    START           = 27,
    STOP            = 28,
    ASPECT          = 29,
    AMBIENT         = 30,
    HAZE            = 31,
    APERTURE        = 32,
    FOCAL_LENGTH    = 33,
    ANTIALIAS       = 34,
    DEPTH           = 35,
    NO_SHADOWS      = 36,
    SAMPLES         = 37,
    JITTER          = 38,
    THRESHOLD       = 39,
    CAUSTICS        = 40,
    NO_EXP_TRANS    = 41,
    NO_ANTIALIAS    = 42,
    NONE            = 43,
    CORNERS         = 44,
    ADAPTIVE        = 45,
    QUICK           = 46,
    BUNCHING        = 47,
    PROJECTION      = 48,
    FLAT            = 49,
    ORTHO           = 50,
    WIDTH           = 51,
    FISHEYE         = 52,
    NO_PARALLAX     = 53,

/* transformations */

    TRANSFORM       = 70,
    TRANS_POP       = 71,
    ROTATE          = 72,
    TRANSLATE       = 73,
    SCALE           = 74,

/* lights */

    LIGHT           = 80,
    TYPE            = 81,
    T_L_POINT           = 82,
    SPHERICAL       = 83,
    SPOT            = 84,
    DIRECTIONAL     = 85,
    MIN_ANGLE       = 86,
    MAX_ANGLE       = 87,
    NOSPEC          = 88,
    COLOR           = 89,
    FALLOFF         = 90,
    DIRECTION       = 91,

/* surfaces */

    SURFACE         = 100,
    DIFFUSE         = 101,
    SPECULAR        = 102,
    TRANSPARENT     = 103,
    SHINE           = 104,
    IOR             = 105,
    FUZZ            = 106,
    TEXTURE         = 107,
    BUMP            = 108,
    T_S_PATTERN         = 109,
    CHECKER         = 110,
    NOISE           = 111,
    TURBULENCE      = 112,
    OFFSET          = 113,
    TERMS           = 114,
    AMP             = 115,
    WAVE            = 116,
    LENGTH          = 117,
    BLEND           = 118,
    IMAGE           = 119,
    ACROSS          = 120,
    DAMPING         = 121,
    PHASE           = 122,

/* primitives */

    CONE            = 130,
    SPHERE          = 131,
    POLYGON         = 132,
    PATCH           = 133,
    RING            = 134,
    VERTEX          = 135,
    T_L_POINTS          = 136,
    CENTER          = 137,
    RADIUS          = 138,
    MIN_RADIUS      = 139,
    MAX_RADIUS      = 140,
    NORMAL          = 141,
    BASE            = 142,
    APEX            = 143,
    BASE_RADIUS     = 144,
    APEX_RADIUS     = 145,

/* clips */

    CLIP            = 160,
    INSIDE          = 161,
    OUTSIDE         = 162,
    GLOBAL_CLIP     = 163,
    CLIP_POP        = 164
};

typedef struct t_token {
    String name;
    TokenType value;
} Token;
