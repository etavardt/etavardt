#pragma once
#include "defs.hpp"

class Wave_3D {
    public:

    Wave_3D(){}
    ~Wave_3D(){}

    Vec center;
    Flt wavelength, amp, /* should be about 0 to .6 */
        damp,            /* damping per wavelength */
        phase;           /* wavelength offset */
    Wave_3D *next;          /* next wave in line */

    static void make_waves(Vec &P, Vec &R, Wave_3D *waves);
};

typedef Wave_3D Wave;

/*
typedef struct t_wave {
    Vec    center;
    Flt    wavelength,
        amp,        //* should be about 0 to .6 * /
        damp,        //* damping per wavelength * /
        phase;        //* wavelength offset * /
    struct t_wave    *next;    //* next wave in line * /
} Wave;
*/