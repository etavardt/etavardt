#pragma once
#include "defs.hpp"
#include "Point_3D.hpp"

typedef struct t_Ray {
    Point P;
    Point D;
} Ray;

inline void RayPoint(Ray &ray, double t, Vec &point) { VecAddS(t, ray.D, ray.P, point); }
