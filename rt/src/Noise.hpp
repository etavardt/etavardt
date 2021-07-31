#pragma once

#include "Vector_3D.hpp"

#define NUMPTS  512

class Noise {
    private:
    static double pts[NUMPTS];
    public:
    static void init_noise(void);
    static double noise1(const Vec &p);
    static void noise3(const Vec &p, Vec &v);
    static double turb1(Vec &p, int lvl);
    static void turb3(Vec &p, Vec &v, int lvl);
    // static void DNoise(const Vec &p, Vec &v);
};
