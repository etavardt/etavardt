#include "Surface_3D.hpp"

Surface_3D *Surface_3D::currentSurface = nullptr; /* surface at current top of stack */

Surface_3D &Surface_3D::operator=(const Surface_3D &s) {
    // Color diff; /* diffuse */
    diff = s.diff;
    // Texmap *tm_diff;
    tm_diff = s.tm_diff;
    // Color spec; /* specular (reflected) */
    spec = s.spec;
    // Texmap *tm_spec;
    tm_spec = s.tm_spec;
    // Color amb; /* ambient */
    amb = s.amb;
    // Texmap *tm_amb;
    tm_amb = s.tm_amb;
    // double shine;    /* specular spot exponent */
    shine = s.shine;
    // Color cshine; /* spec spot color */
    cshine = s.cshine;
    // Color trans;  /* transparency */
    trans = s.trans;
    // Texmap *tm_trans;
    tm_trans = s.tm_trans;
    // double ior;               /* index of refraction */
    ior = s.ior;
    // double fuzz;              /* surface_3D fuzz */
    fuzz = s.fuzz;
    // int flags;             /* is this surface_3D valid for shadow caching */
    flags = s.flags;
    // struct t_texture *tex; /* ptr for color texture */
    tex = s.tex;
    // struct t_bump *bump;   /* ptr for surface_3D normal texture */
    bump = s.bump;
    // Matrix matrix;         /* transformation matrix */
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            matrix[i][j] = s.matrix[i][j];
        }
    }
    return *this;
}
