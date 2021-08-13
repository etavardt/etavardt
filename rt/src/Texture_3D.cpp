/*
***************************************************************************
*                                                                         *
*                             Bob Ray Tracer                              *
*                                                                         *
*   Texture.C = Procedural textures and image texture mapping functions.  *
*                                                                         *
*       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     *
*                                                                         *
*       ALL RIGHTS RESERVED.   This software is published, but is NOT     *
*         Public Domain and remains the propery of ALGORITHM, Inc.,       *
*   Christopher D. Watkins and Stephen B. Coy.  This software may not be  *
*  reproduced or integrated into other packages without the prior written *
*          consent of Christopher D. Watkins and Stephen B. Coy.          *
*                                                                         *
*                       Requires: defs.h, extern.h                        *
*                                                                         *
***************************************************************************

    calling sequence for all functions is

    float = tex_func(P, *Texture);

    The return value is a number between 0.0 and 1.0 which determines
    the relative weights of the composite surface.
*/

#include "Texture_3D.hpp"

#include <cmath>

#include "Bob.hpp"
#include "BobMath.hpp"
#include "Exception.hpp"
#include "Surface_3D.hpp"
#include "Noise.hpp"

/*
    Check -- create a checkerboard, strange
*/
double Texture_3D::tex_checker(const Point &P) {
    int i = 0;
    int p[3] = {0,0,0};
    Vec point;
    double blur = HUGE_NUM;

    for (i = 0; i < 3; i++) {
        if (scale[i] == 0) {
            p[i] = 0;
        } else {
            point[i] = (P[i] + trans[i]) / scale[i];
            p[i] = floor(point[i]);
            point[i] -= p[i];
            if (point[i] > .5)
                point[i] = 1.0 - point[i];
            blur = bMath::min(point[i], blur);
        }
    }

    blur *= 2.0;
    if (blur >= blur)
        blur = 0.0;
    else
        blur = (1.0 - blur / blur) / 2.0;

    if ((p[0] + p[1] + p[2]) & 0x01) {
        return 1.0 - blur;
    } else {
        return blur;
    }
}

/*
    spherical
*/
double Texture_3D::tex_spherical(const Point &P) {
    int i = 0;
    double r = 0.0, dist = 0.0;
    Vec p;

    r = r1 + r2;
    dist = 0.0;
    for (i = 0; i < 3; i++) {
        if (scale[i] == 0) {
            p[i] = 0;
        } else {
            p[i] = (P[i] - trans[i]) / scale[i];
        }
        dist += p[i] * p[i];
    }
    dist = sqrt(dist);  /* whata bummer! */
    dist += r1 / 2; /* center first color */
    dist = fmod(dist, r);

    if (dist < r1) { /* still in the r1 range */
        dist = dist / r1;
        if (dist > 0.5) {
            dist = 1.0 - dist; /* fold if needed */
        }
        if (dist >= blur / 2.0) { /* we're in the flat */
            return 1.0;
        } else {
            return 0.5 + dist / blur;
        }
    } else { /* into the r2 range */
        dist = (dist - r1) / r2;
        if (dist > 0.5) {
            dist = 1.0 - dist; /* fold if needed */
        }
        if (dist >= blur / 2.0) { /* we're in the flat */
            return 0.0;
        } else {
            return 0.5 - dist / blur;
        }
    }
} /* end of spherical */

/*
    noise - BANG, SPLAT, POW!!!
*/
double Texture_3D::tex_noise(const Point &P) {
    double result = 0.0;
    Vec p;

    for (int i = 0; i < 3; i++) {
        if (scale[i] == 0) {
            p[i] = 0;
        } else {
            p[i] = (P[i] - trans[i]) / scale[i];
        }
    }

    result = Noise::turb1(p, terms);

    return result;

} /* end of tex_noise() */

/*
    map_fix() -- fill in the surface structure element(s) based
        on the point of intersection and the texture map.
*/
//           along with tile seeing as this is the only place that calls it
void Texture_3D::map_fix(Surface_3D &surf, const Point &P) {
    double x = 0.0, y = 0.0; /* image intersection */

    if (surf.flags & S_TM_DIFF) { /* we've got a diffuse map */
        surf.tm_diff->tex_project(P, &x, &y);
        surf.tm_diff->tile(&x, &y);
        surf.tm_diff->get_map_entry(x, y, surf.diff);
    }
    if (surf.flags & S_TM_SPEC) { /* we've got a specular map */

        surf.tm_spec->tex_project(P, &x, &y);
        surf.tm_spec->tile(&x, &y);
        surf.tm_spec->get_map_entry(x, y, surf.spec);
    }
    if (surf.flags & S_TM_TRANS) { /* we've got a transparent map */
        surf.tm_trans->tex_project(P, &x, &y);
        surf.tm_trans->tile(&x, &y);
        surf.tm_trans->get_map_entry(x, y, surf.trans);
    }
    if (surf.flags & S_TM_AMB) { /* we've got a ambient map */
        surf.tm_amb->tex_project(P, &x, &y);
        surf.tm_amb->tile(&x, &y);
        surf.tm_amb->get_map_entry(x, y, surf.amb);
    }
} /* end of map_fix() */

/*
    tex_fix() -- figure out which surface to use as point P
*/
//    Surface *surf;
//    Point   P, OP;  /* translated and original point */
void Texture_3D::tex_fix(Surface_3D &surf, Point &P, Point &OP) {
    double w0 = 0.0, w1 = 0.0;
    Texture_3D *texture = surf.tex;
    Surface_3D *surf0 = nullptr;
    Surface_3D *surf1 = nullptr;
    Point tmp, p_in, p_out;

    if (texture == nullptr) {
        return; // TCE: the following lines will not be hit due to this return (Note: the original code had this.)
        //        cerr << "Fooey, null pointer for texture structure." << endl;
        //        throw Exception("Thrown from tex_fix");
    } else {
        surf0 = texture->surf[0];
        surf1 = texture->surf[1];

        if (surf0 == nullptr) {
            return;
            //        cerr << "Fooey, null pointer for surf0 structure.\n");
            //        throw Exception("Thrown from tex_fix");
        }
        if (surf1 == nullptr) {
            return;
            //        cerr << "Fooey, null pointer for surf1 structure.\n");
            //        throw Exception("Thrown from tex_fix");
        }
    }

    VecCopy(P, tmp); /* save point */

    if (texture->turbulence) { /* tweek P if turbulent */
        VecCopy(P, p_in);
        for (int i = 0; i < 3; i++) {
            p_in[i] *= texture->turbulence->scale[i];
        }
        Noise::turb3(p_in, p_out, texture->turbulence->terms);
        VecAddS(texture->turbulence->amp, p_out, P, P);
    }

    if (surf0->tex) /* recurse down left branch */
        tex_fix(*surf0, P, OP);
    if (surf1->tex) /* recurse down right branch */
        tex_fix(*surf1, P, OP);

    //    w0 = (texture->func)(P, *texture);
    w0 = texture->apply_pattern(P);
    w0 = bMath::abs(w0);
    if (w0 < 0.0) {
        w0 = 0.0;
    } else if (w0 > 1.0) {
        w0 = 1.0;
    }
    w1 = 1.0 - w0;

    /* handle any image mapping */
    if (surf0->flags & S_TM_MAPPING) {
        map_fix(*surf0, OP);
    }
    if (surf1->flags & S_TM_MAPPING) {
        map_fix(*surf1, OP);
    }
    surf.diff = (surf0->diff * w0) + (surf1->diff * w1);
    surf.spec = (surf0->spec * w0) + (surf1->spec * w1);
    surf.trans = (surf0->trans * w0) + (surf1->trans * w1);
    surf.amb = (surf0->amb * w0) + (surf1->amb * w1);
    surf.cshine = (surf0->cshine * w0) + (surf1->cshine * w1);

    surf.ior = w0 * surf0->ior + w1 * surf1->ior;
    surf.fuzz = w0 * surf0->fuzz + w1 * surf1->fuzz;
    surf.shine = w0 * surf0->shine + w1 * surf1->shine;

    /* both cache and no_antialias flags must be set for
       textured surface to have them */

    if (surf0->flags & surf1->flags & S_CACHE) {
        surf.flags |= S_CACHE;
    } else {
        surf.flags &= ~S_CACHE;
    }
    if (surf0->flags & surf1->flags & S_NO_ANTIALIAS) {
        surf.flags |= S_NO_ANTIALIAS;
    } else {
        surf.flags &= ~S_NO_ANTIALIAS;
    }

    VecCopy(tmp, P); /* restore point */

} /* end of tex_fix() */

double Texture_3D::apply_pattern(const Point &p) {
    double ret_val = 0;
    switch (pat_type) {

    case CHECKER_PAT:
        ret_val = tex_checker(p);
        break;
    case SPHERICAL_PAT:
        ret_val = tex_spherical(p);
        break;
    case NOISE_PAT:
        ret_val = tex_noise(p);
        break;
    case UNKNOWN_PAT:
        /* code */
        break;

    default:
        break;
    }
    return ret_val;
}
