#pragma once

#include "Ray_3D.hpp"
#include "Object_3D.hpp"
#include "Light_3D.hpp"
#include "config.hpp"

//
// This class will incorperate trace, inter, shade, and shadow into it.
//
typedef Color Palette[256];

typedef struct t_background {
    Color color;
    Vec up;
    Palette pal;
} Background;

class RayTrace_3D {
    private:

    // num and den used by intersect and checkAndEnqueue in inter.cpp
    static double    num[NSLABS];
    static double    den[NSLABS];

    protected:

    public:
    static Background    background;            /* background color structure */
    static Color         ambient;               /* global ambient light */
    static Color         hazeColor;             /* color of haze */
    static double        hazeDensity;
    static unsigned long nReflected;
    static unsigned long nRefracted;
    static unsigned long nShadows;
    static unsigned long nRays;
    static int           deepest;           /* deepest level reached */
    static int           maxlevel;

    // inter.cpp
    static void checkAndEnqueue(Object *obj, double maxdist);
    static int intersect(Ray *ray, Isect &hit, double maxdist, Object *lastObjHit);
    // trace.cpp
    static void bkg(Vec &dir, Color &col);
    static double trace(int level, double weight, Ray *ray, Color &color, double ior, Object *obj);
    // shade.cpp
    static void reflect(const Vec &I, const Vec &N, Vec &R, double dot);
    static int refract(double eta, const Vec &I, const Vec &N, Vec &T, double dot);
    static void shade(int level, double weight, Point &P, Vec &N, Vec &I, Isect &hit, Color &col, double ior);
    // shadow.cpp
    static int shadow (Ray *ray , Isect &hit , double tmax , Color &color , int level , Light_3D &lptr, int inside);
    static int sShadow (Ray *ray , Isect &hit , double tmax , Color &color , int level , Light_3D &lptr, int inside);

};
