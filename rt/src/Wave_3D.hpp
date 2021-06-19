#pragma once
#include "defs.hpp"

typedef struct t_wave {
    Vec    center;
    Flt    wavelength,
        amp,        /* should be about 0 to .6 */
        damp,        /* damping per wavelength */
        phase;        /* wavelength offset */
    struct t_wave    *next;    /* next wave in line */
} Wave;