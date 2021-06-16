#pragma once
#include "defs.hpp"

typedef struct t_clip {
    Vec center,
        normal,
        apex, base;
    Flt radius1,
        radius2,
        length;
    int type;
    struct t_clip *next;
} Clip;

typedef struct t_global_clip {
    Clip                    *clip;
    struct t_global_clip    *next;
} GlobalClip;
