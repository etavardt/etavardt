/*
*******************************************
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
*******************************************

    calling sequence for all functions is

    float = tex_func(P, *Texture);

    The return value is a number between 0.0 and 1.0 which determines
    the relative weights of the composite surface.
*/

#include "Texture_3D.hpp"

#include <cmath>

#include "Bob.hpp"
#include "Exception.hpp"
#include "Surface_3D.hpp"
#include "defs.hpp"
#include "extern.hpp"
#include "proto.hpp"

/*
    Check -- create a checkerboard, strange
*/
double Texture_3D::tex_checker(const Point &P, const Texture_3D &tex) {
    int i = 0;
    int p[3];
    Vec point;
    double blur;

    blur = HUGE_NUM;

    for (i = 0; i < 3; i++) {
        if (tex.scale[i] == 0) {
            p[i] = 0;
        } else {
            point[i] = (P[i] + tex.trans[i]) / tex.scale[i];
            p[i] = floor(point[i]);
            point[i] -= p[i];
            if (point[i] > .5)
                point[i] = 1.0 - point[i];
            blur = bMath::min(point[i], blur);
        }
    }

    blur *= 2.0;
    if (blur >= tex.blur)
        blur = 0.0;
    else
        blur = (1.0 - blur / tex.blur) / 2.0;

    if ((p[0] + p[1] + p[2]) & 0x01) {
        return 1.0 - blur;
    } else {
        return blur;
    }
}

/*
    spherical
*/

double Texture_3D::tex_spherical(const Point &P, const Texture_3D &tex) {
    int i;
    double r, dist;
    Vec p;

    r = tex.r1 + tex.r2;
    dist = 0.0;
    for (i = 0; i < 3; i++) {
        if (tex.scale[i] == 0) {
            p[i] = 0;
        } else {
            p[i] = (P[i] - tex.trans[i]) / tex.scale[i];
        }
        dist += p[i] * p[i];
    }
    dist = sqrt(dist);  /* whata bummer! */
    dist += tex.r1 / 2; /* center first color */
    dist = fmod(dist, r);

    if (dist < tex.r1) { /* still in the r1 range */
        dist = dist / tex.r1;
        if (dist > 0.5) {
            dist = 1.0 - dist; /* fold if needed */
        }
        if (dist >= tex.blur / 2.0) { /* we're in the flat */
            return 1.0;
        } else {
            return 0.5 + dist / tex.blur;
        }
    } else { /* into the r2 range */
        dist = (dist - tex.r1) / tex.r2;
        if (dist > 0.5) {
            dist = 1.0 - dist; /* fold if needed */
        }
        if (dist >= tex.blur / 2.0) { /* we're in the flat */
            return 0.0;
        } else {
            return 0.5 - dist / tex.blur;
        }
    }

} /* end of spherical */

/*
    noise - BANG, SPLAT, POW!!!
*/

double Texture_3D::tex_noise(const Point &P, const Texture_3D &tex) {
    int i;
    double result;
    Vec p;

    for (i = 0; i < 3; i++) {
        if (tex.scale[i] == 0) {
            p[i] = 0;
        } else {
            p[i] = (P[i] - tex.trans[i]) / tex.scale[i];
        }
    }

    result = turb1(p, tex.terms);

    return result;

} /* end of tex_noise() */

/*
    get_map_entry() -- Given a texture structure and the indices
        into the map this function fills in the color at that
        point.  Note that indices are actually backwards.
*/
//TODO: TCE: Belongs in Surface_3D, move it there once color is fixed again the output files
void Texture_3D::get_map_entry(Texmap &tm, double x, double y, Color &color) {
    double r, g, b;
    int i, j, map_index;

    /* get integer indices */
    i = x * (tm.xres);
    j = y * (tm.xres);

    color.r = tm.red[j][i] / 255.0;
    color.g = tm.grn[j][i] / 255.0;
    color.b = tm.blu[j][i] / 255.0;

} /* end of get_map_entry() */

/*
    tex_project() -- For the texture defined by surf.tm_???
        project the point P onto the image plane and return
        the indices for the image
*/
//TODO: TCE: Belongs in Surface_3D or Texmap_3D, move it there once color is fixed again the output files
void Texture_3D::tex_project(Texmap &tm, Point P, double *x, double *y) {
    Point PP, /* point projected onto plane of image */
        V;
    double dot;
//    Texmap tm(_tm);// = _tm;
    /* project intersection point onto image plane */
    VecSub(P, tm.position, V);
    dot = VecDot(tm.normal, V);
    VecAddS(-dot, tm.normal, P, PP);

    /* calc offsets in across and down directions */
    VecSub(PP, tm.position, V);
    dot = VecDot(tm.across, V);
    *x = dot / tm.scale;
    dot = VecDot(tm.down, V);
    *y = dot * (double)tm.yres / tm.xres / tm.scale;
    *y = dot / tm.scale;

} /* end of tex_project() */

/*
    tile() -- Take the raw indices and based on the tile pattern return
        the indices that are within the image bounds.
*/
void Texture_3D::tile(const Texmap &tm, double *x, double *y) {
    *x = fmod(*x, 1.0);
    if (*x < 0.0) {
        *x += 1.0;
    }
    *y = fmod(*y, (double)tm.yres / (double)tm.xres);
    if (*y < 0.0) {
        *y += (double)tm.yres / (double)tm.xres;
    }
} /* end of tile() */

/*
    map_fix() -- fill in the surface structure element(s) based
        on the point of intersection and the texture map.
*/
//TODO: TCE: Belongs in Surface_3D, move it there once color is fixed again the output files
//           along with tile seeing as this is the only place that calls it
void Texture_3D::map_fix(Surface_3D &surf, const Point &P) {
    double x, y; /* image intersection */

    if (surf.flags & S_TM_DIFF) { /* we've got a diffuse map */
        tex_project(*surf.tm_diff, P, &x, &y);
        tile(*surf.tm_diff, &x, &y);
        get_map_entry(*surf.tm_diff, x, y, surf.diff);
    }
    if (surf.flags & S_TM_SPEC) { /* we've got a specular map */
        tex_project(*surf.tm_spec, P, &x, &y);
        tile(*surf.tm_spec, &x, &y);
        get_map_entry(*surf.tm_spec, x, y, surf.spec);
    }
    if (surf.flags & S_TM_TRANS) { /* we've got a transparent map */
        tex_project(*surf.tm_trans, P, &x, &y);
        tile(*surf.tm_trans, &x, &y);
        get_map_entry(*surf.tm_trans, x, y, surf.trans);
    }
    if (surf.flags & S_TM_AMB) { /* we've got a ambient map */
        tex_project(*surf.tm_amb, P, &x, &y);
        tile(*surf.tm_amb, &x, &y);
        get_map_entry(*surf.tm_amb, x, y, surf.amb);
    }
} /* end of map_fix() */

/*
    tex_fix() -- figure out which surface to use as point P
*/
//TODO: TCE: Belongs in Surface_3D, move it there once color is fixed again the output files
//    Surface *surf;
//    Point   P, OP;  /* translated and original point */
void Texture_3D::tex_fix(Surface_3D &surf, Point &P, Point &OP) {
    int i = 0;
    double w0, w1;
    Surface_3D *surf0, *surf1;
    Texture_3D *texture;
    Point tmp, p_in, p_out;

    texture = surf.tex;
    surf0 = texture->surf[0];
    surf1 = texture->surf[1];

    if (texture == NULL) {
        return; // TCE: the following lines will not be hit due to this return (Note: the original code had this.)
        //        cerr << "Fooey, null pointer for texture structure." << endl;
        //        throw Exception("Thrown from tex_fix");
    }
    if (surf0 == NULL) {
        return;
        //        cerr << "Fooey, null pointer for surf0 structure.\n");
        //        throw Exception("Thrown from tex_fix");
    }
    if (surf1 == NULL) {
        return;
        //        cerr << "Fooey, null pointer for surf1 structure.\n");
        //        throw Exception("Thrown from tex_fix");
    }

    VecCopy(P, tmp); /* save point */

    if (texture->turbulence) { /* tweek P if turbulent */
        VecCopy(P, p_in);
        for (i = 0; i < 3; i++) {
            p_in[i] *= texture->turbulence->scale[i];
        }
        turb3(p_in, p_out, texture->turbulence->terms);
        VecAddS(texture->turbulence->amp, p_out, P, P);
    }

    if (surf0->tex) /* recurse down left branch */
        tex_fix(*surf0, P, OP);
    if (surf1->tex) /* recurse down right branch */
        tex_fix(*surf1, P, OP);

    //    w0 = (texture->func)(P, *texture);
    w0 = apply_pattern(P, *texture);
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

double Texture_3D::apply_pattern(const Point &p, const Texture_3D &t) {
    double ret_val = 0;
    switch (pat_type) {

    case CHECKER_PAT:
        ret_val = tex_checker(p, t);
        break;
    case SPHERICAL_PAT:
        ret_val = tex_spherical(p, t);
        break;
    case NOISE_PAT:
        ret_val = tex_noise(p, t);
        break;
    case UNKNOWN_PAT:
        /* code */
        break;

    default:
        break;
    }
    return ret_val;
}
