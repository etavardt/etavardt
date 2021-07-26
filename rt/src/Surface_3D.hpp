#pragma once
#include "Color.hpp"
#include "Texture_3D.hpp"
#include "Texmap_3D.hpp"
#include "Matrix_3D.hpp"

//class Texture_3D;
class Bump;

class Surface_3D {
  public:
    Surface_3D() : shine(0.0), ior(1.0), fuzz(0.0), flags(S_CACHE), tex(nullptr), bump(nullptr), tm_diff(nullptr), tm_spec(nullptr), tm_trans(nullptr), tm_amb(nullptr) {}
    ~Surface_3D() {}

    Color       diff;     /* diffuse */
    Texmap     *tm_diff;
    Color       spec;     /* specular (reflected) */
    Texmap     *tm_spec;
    Color       amb;      /* ambient */
    Texmap     *tm_amb;
    double      shine;    /* specular spot exponent */
    Color       cshine;   /* spec spot color */
    Color       trans;    /* transparency */
    Texmap     *tm_trans;
    double      ior;      /* index of refraction */
    double      fuzz;     /* surface fuzz */
    int         flags;    /* is this surface valid for shadow caching */
    Texture_3D *tex;      /* ptr for color texture */
    Bump       *bump;     /* ptr for surface normal texture */
    Matrix      matrix;   /* transformation matrix */

    Surface_3D &operator=(const Surface_3D &s);

    static Surface_3D *currentSurface; /* surface at current top of stack */
};
