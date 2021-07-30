#pragma once
#include "defs.hpp"
#include "Vector_3D.hpp"

class Wave_3D {
    public:

    Wave_3D() : wavelength(0.0), amp(0.0), damp(0.0), phase(0.0), next(nullptr) {}
    ~Wave_3D(){}

    Vec center;
    double wavelength, amp, /* should be about 0 to .6 */
        damp,            /* damping per wavelength */
        phase;           /* wavelength offset */
    Wave_3D *next;          /* next wave in line */

    static void make_waves(const Vec &P, Vec &R, Wave_3D *waves);
};

typedef Wave_3D Wave;
