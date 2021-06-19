#pragma once
#include "Turbulence_3D.hpp"
#include "Wave_3D.hpp"
#include "defs.hpp"

typedef struct t_bump {
    Vec trans,        /* apply to resulting normal */
        scale;
    Turbulence      *turbulence;
    Wave            *waves;
} Bump;