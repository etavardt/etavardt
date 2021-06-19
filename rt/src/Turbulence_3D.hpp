#pragma once
#include "defs.hpp"

typedef struct t_turbulence {
    int     terms;          /* # of terms in the series */
    Vec     trans,          /* pre-process numbers */
        scale;
    Flt     amp;            /* post turbulence amplification */
} Turbulence;