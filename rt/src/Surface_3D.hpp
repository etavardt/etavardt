#pragma once
#include "Color.hpp"
#include "Texture_3D.hpp"
#include "Texmap_3D.hpp"
#include "Matrix_3D.hpp"

//class Texture_3D;
class Bump;

class Surface_3D {
  public:
    double      shine;    /* specular spot exponent */
    double      ior;      /* index of refraction */
    double      fuzz;     /* surface fuzz */
    int         flags;    /* is this surface valid for shadow caching */
    Texture_3D *tex;      /* ptr for color texture */
    Bump       *bump;     /* ptr for surface normal texture */
    Texmap     *tm_diff;
    Texmap     *tm_spec;
    Texmap     *tm_amb;
    Texmap     *tm_trans;

    Color       diff;     /* diffuse */
    Color       spec;     /* specular (reflected) */
    Color       amb;      /* ambient */
    Color       cshine;   /* spec spot color */
    Color       trans;    /* transparency */
    Matrix      matrix;   /* transformation matrix */

    Surface_3D() : shine(0.0), ior(1.0), fuzz(0.0), flags(S_CACHE), tex(nullptr), bump(nullptr), tm_diff(nullptr), tm_spec(nullptr), tm_amb(nullptr), tm_trans(nullptr) {}
    ~Surface_3D() {}

    Surface_3D &operator=(const Surface_3D &s);

    static Surface_3D *currentSurface; /* surface at current top of stack */
};
