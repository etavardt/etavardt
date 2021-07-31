#pragma once

#include "Vector_3D.hpp"

extern void init_noise(void);
extern double noise1(const Vec &p);
extern void noise3(const Vec &p, double *v);
extern double turb1(Vec &p, int lvl);
extern void turb3(Vec &p, Vec &v, int lvl);
