/*
�������������������������������������������
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�   Texture.C = Procedural textures and image texture mapping functions.  �
�                                                                         �
�       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     �
�                                                                         �
�       ALL RIGHTS RESERVED.   This software is published, but is NOT     �
�         Public Domain and remains the propery of ALGORITHM, Inc.,       �
�   Christopher D. Watkins and Stephen B. Coy.  This software may not be  �
�  reproduced or integrated into other packages without the prior written �
�          consent of Christopher D. Watkins and Stephen B. Coy.          �
�                                                                         �
�                       Requires: defs.h, extern.h                        �
�                                                                         �
�������������������������������������������

    calling sequence for all functions is

    float = tex_func(P, *Texture);

    The return value is a number between 0.0 and 1.0 which determines
    the relative weights of the composite surface.
*/

#include "Bob.hpp"
#include "Exception.hpp"
#include "Stats.hpp"
#include "Texture_3D.hpp"
#include "Surface_3D.hpp"
#include "defs.hpp"
#include "extern.hpp"
#include "proto.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>
/*
    Check -- create a checkerboard, strange
*/

Flt tex_checker(const Point P, const Texture &tex) {
    int i = 0;
    int p[3];
    Vec point;
    Flt blur;

    blur = HUGE;

    for (i = 0; i < 3; i++) {
        if (tex.scale[i] == 0) {
            p[i] = 0;
        } else {
            point[i] = (P[i] + tex.trans[i]) / tex.scale[i];
            p[i] = floor(point[i]);
            point[i] -= p[i];
            if (point[i] > .5)
                point[i] = 1.0 - point[i];
            blur = MIN(point[i], blur);
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

Flt tex_spherical(const Point P, const Texture &tex) {
    int i;
    Flt r, dist;
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
    dist = sqrt(dist);   /* whata bummer! */
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

Flt tex_noise(const Point P, const Texture &tex) {
    int i;
    Flt result;
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

void get_map_entry(Texmap &tm, Flt x, Flt y, Color &color) {
    Flt r, g, b;
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

void tex_project(const Texmap &tm, Point P, Flt *x, Flt *y) {
    Point PP, /* point projected onto plane of image */
        V;
    Flt dot;

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

void tile(const Texmap &tm, Flt *x, Flt *y) {
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

void map_fix(Surface_3D &surf, Point P) {
    Flt x, y; /* image intersection */

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
    tex_read_img() -- Read a .img file into a texture map structure
*/

void tex_read_img(const String &filename, Texmap &tm) {
    FILE *fp;
    int w, h, /* width and height */
        i, j, cnt, red, grn, blu;

    fp = env_fopen(filename, "rb");
    if (!fp) {
        cerr << "Error opening file " << filename << " for texture mapping." << endl;
        throw Exception("Thrown from tex_read_img");
    }

    /* get width and height from header */
    w = fgetc(fp) << 8;
    w += fgetc(fp);
    h = fgetc(fp) << 8;
    h += fgetc(fp);

    /* waste other stuff */
    fgetc(fp);
    fgetc(fp);
    fgetc(fp);
    fgetc(fp);
    fgetc(fp);
    fgetc(fp);

    /* allocate memory for image in RAM */
    typedef unsigned char *Lines;
    typedef unsigned char rows;

    tm.red = new Lines[h]();
    Stats::trackMemoryUsage(sizeof(Lines[h]));
    //    tm->red = (unsigned char **)vmalloc(sizeof(unsigned char *) * h);
    Bob::getApp().parser.ptrchk(tm.red, "image texture map");
    tm.grn = new Lines[h]();
    Stats::trackMemoryUsage(sizeof(Lines[h]));
    //    tm->grn = (unsigned char **)vmalloc(sizeof(unsigned char *) * h);
    Bob::getApp().parser.ptrchk(tm.grn, "image texture map");
    tm.blu = new Lines[h]();
    Stats::trackMemoryUsage(sizeof(Lines[h]));
    //    tm->blu = (unsigned char **)vmalloc(sizeof(unsigned char *) * h);
    Bob::getApp().parser.ptrchk(tm.blu, "image texture map");
    for (j = 0; j < h; j++) {
        tm.red[j] = new rows[w]();
        Stats::trackMemoryUsage(sizeof(rows[w]));
        //        tm->red[j] = (unsigned char *)vmalloc(sizeof(unsigned char) * w);
        Bob::getApp().parser.ptrchk(tm.red[j], "image texture map");

        tm.grn[j] = new rows[w]();
        Stats::trackMemoryUsage(sizeof(rows[w]));
        //        tm->grn[j] = (unsigned char *)vmalloc(sizeof(unsigned char) * w);
        Bob::getApp().parser.ptrchk(tm.grn[j], "image texture map");

        tm.blu[j] = new rows[w]();
        Stats::trackMemoryUsage(sizeof(rows[w]));
        //        tm->blu[j] = (unsigned char *)vmalloc(sizeof(unsigned char) * w);
        Bob::getApp().parser.ptrchk(tm.blu[j], "image texture map");
    }

    /* read in the image */
    for (j = 0; j < h; j++) {
        i = 0;
        while (i < w) {
            cnt = fgetc(fp) & 0xff;
            blu = fgetc(fp) & 0xff;
            grn = fgetc(fp) & 0xff;
            red = fgetc(fp) & 0xff;
            while (cnt) {
                tm.red[j][i] = red;
                tm.grn[j][i] = grn;
                tm.blu[j][i] = blu;
                i++;
                cnt--;
            }
        }
    } /* end of loop for each scan line */

    fclose(fp);

    tm.xres = w;
    tm.yres = h;
} /* end of tex_read_img() */

/*
    tex_fix() -- figure out which surface to use as point P
*/

//    Surface *surf;
//    Point   P, OP;  /* translated and original point */
void tex_fix(Surface_3D &surf, Point P, Point OP) {
    int i = 0;
    Flt w0, w1;
    Surface_3D *surf0, *surf1;
    Texture *texture;
    Point tmp, p_in, p_out;

    texture = surf.tex;
    surf0 = texture->surf[0];
    surf1 = texture->surf[1];

    if (texture == NULL) {
        return; //TCE: the following lines will not be hit due to this return (Note: the original code had this.)
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

    w0 = (texture->func)(P, *texture);
    w0 = ABS(w0);
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
    /*
        for(i=0; i<3; i++) {
            surf.diff[i]   = w0 * surf0->diff[i]   + w1 * surf1->diff[i];
            surf.spec[i]   = w0 * surf0->spec[i]   + w1 * surf1->spec[i];
            surf.trans[i]  = w0 * surf0->trans[i]  + w1 * surf1->trans[i];
            surf.amb[i]    = w0 * surf0->amb[i]    + w1 * surf1->amb[i];
            surf.cshine[i] = w0 * surf0->cshine[i] + w1 * surf1->cshine[i];
        }
        */
    // surf.diff.r = w0 * surf0-diff.r + w1 * surf1->diff.r;
    surf.diff   = (surf0->diff   * w0) + (surf1->diff   * w1);
    surf.spec   = (surf0->spec   * w0) + (surf1->spec   * w1);
    surf.trans  = (surf0->trans  * w0) + (surf1->trans  * w1);
    surf.amb    = (surf0->amb    * w0) + (surf1->amb    * w1);
    surf.cshine = (surf0->cshine * w0) + (surf1->cshine * w1);

    surf.ior   = w0 * surf0->ior   + w1 * surf1->ior;
    surf.fuzz  = w0 * surf0->fuzz  + w1 * surf1->fuzz;
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
