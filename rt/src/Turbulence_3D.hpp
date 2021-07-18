#pragma once
#include "defs.hpp"
#include "Vector_3D.hpp"

typedef struct t_turbulence {
    int terms; /* # of terms in the series */
    Vec trans, /* pre-process numbers */
        scale;
    double amp; /* post turbulence amplification */
} Turbulence;