#pragma once

#include "Vector_3D.hpp"
#include "Turbulence_3D.hpp"
#include "Wave_3D.hpp"
#include "defs.hpp"

class Bump {
    public:
    Bump() : turbulence(nullptr), waves(nullptr) {}
    ~Bump(){}

    Vec trans,        /* apply to resulting normal */
        scale;
    Turbulence *turbulence;
    Wave       *waves;
};
