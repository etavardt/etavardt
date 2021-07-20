#pragma once
#include "defs.hpp"
#include "Vector_3D.hpp"

typedef struct t_clip {
    Vec center,
        normal,
        apex, base;
    double radius1,
        radius2,
        length;
    int type;
    t_clip *next;
} Clip;

typedef struct t_global_clip {
    Clip                    *clip;
    t_global_clip    *next;
} GlobalClip;

extern void init_noise (void);
extern double  noise1 (const Vec &p);
extern void noise3 (const Vec &p , double *v);
extern double  turb1 (Vec &p , int lvl);
extern void turb3 (Vec &p , Vec &v , int lvl);
