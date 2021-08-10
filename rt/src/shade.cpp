/*
***************************************************************************
*                                                                         *
*                             Bob Ray Tracer                              *
*                                                                         *
*       Shade.C = Given the intersection point shade() calculates the     *
*          actual color at that point.  This may involve                  *
*                 extra calls to trace for reflection and refraction.     *
*                                                                         *
*       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     *
*                                                                         *
*       ALL RIGHTS RESERVED.   This software is published, but is NOT     *
*         Public Domain and remains the propery of ALGORITHM, Inc.,       *
*   Christopher D. Watkins and Stephen B. Coy.  This software may not be  *
*  reproduced or integrated into other packages without the prior written *
*          consent of Christopher D. Watkins and Stephen B. Coy.          *
*                                                                         *
*                  Requires: defs.h, extern.h,c onfig.h                   *
*                                                                         *
***************************************************************************
*/
#include "RayTrace_3D.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory.h>

#include "Bob.hpp"
#include "BobMath.hpp"
#include "Bump_3D.hpp"
#include "Color.hpp"
#include "Isect_3D.hpp"
#include "Light_3D.hpp"
#include "Clip_3D.hpp"
#include "Noise.hpp"
#include "config.hpp"

#include "extern.hpp"


#define DEFAULT_IOR (1.0)

unsigned long RayTrace_3D::nReflected = 0;
unsigned long RayTrace_3D::nRefracted = 0;

//double      minweight = MINWEIGHT;

/*
    reflect -- given an incident vector I, and the normal N,
        calculate the direction of the reflected ray R.
*/
// Vec I, N, R;
// double dot;        /* -VecDot(I,N) */
void RayTrace_3D::reflect(const Vec &I, const Vec &N, Vec &R, double dot) {
    double len;

    len = 2.0 * dot;
    VecAddS(len, N, I, R);
}

/*
    refract(ior_ratio, I, N, T) calculates direction of refracted
        angle, T.  Returns 0 if total internal reflection occurs,
        1 otherwise.
*/
// double eta;    /* ratio of old/new iors */
// Vec I,      /* incident vector */
//     N,      /* surface normal */
//     T;      /* transmitted vector (calculated) */
// double dot;    /* -VecDot(I, N) */
int RayTrace_3D::refract(double eta, const Vec &I, const Vec &N, Vec &T, double dot) {
    double c1, cs2;

    if (eta == 1.0) { /* bail out early */
        VecCopy(I, T);
        return 1;
    }

    c1 = dot;
    cs2 = 1.0 - eta * eta * (1.0 - c1 * c1);
    if (cs2 < 0.0) {
        reflect(I, N, T, dot);
        return 0; /* total internal reflection */
    }
    cs2 = eta * c1 - sqrt(cs2);
    VecComb(eta, I, cs2, N, T);

    return 1;
}

// TODO: TCE:Should fall under Ray or light?
/*
    Shade(level, weight, P, N, I, hit, col, ior)

    level    current depth in shading tree
    weight    used for early cut off
    P    intersection point
    N       surface normal at intersection
    I    incident vector, points along ray toward intersection
    hit    what we hit
    col    color to return
    ior    current ior
*/
void RayTrace_3D::shade(int level, double weight, Point &P, Vec &N, Vec &I, const Isect &hit, Color &col, double ior) {
    /* the following locals have been declared static to help */
    /* shrink the amount of stack spaced needed for recursive */
    /* calls to shade() */

    static int inside;   /* new inside/outside determination */
    Point tex_P,         /* transformed point for textures */
        L, R, tmpV;      /* direction to light vector */
    Vec fuzz_N;          /* fuzzed, bumped and waved surface normal */
    static Color c_diff, /* diffuse color */
        c_dist,          /* light color scaled by distance */
        c_shadow;        /* shadow color */
    int R_calced; /* Has R already be calculated? */
    double    dot;        /* cos of incident angle */
    static Light *cur_light; /* ptr to current light we're looking at */

    Isect nhit;
    Ray tray;
    Color tcol;
    double t;
    Surface_3D *surf;

    if (level == 0) { /* assume all eye rays start outside */
        inside = 0;
    }

    surf = hit.isect_prim->o_surf; /* pick out surf we hit */

    /* transform point if needed */

    if (surf->flags & S_TRANSFORM) {
        surf->matrix.trans_vector(P, tex_P);
    } else {
        VecCopy(P, tex_P);
    }

    /* handle any texturing up front */

    if (surf->tex) {
        if (surf->tex->fuzz > 0.0) {
            MakeVector(bMath::rnd(), bMath::rnd(), bMath::rnd(), R);
            if (bMath::rnd() > 0.5)
                R[0] = -R[0];
            if (bMath::rnd() > 0.5)
                R[1] = -R[1];
            if (bMath::rnd() > 0.5)
                R[2] = -R[2];
            VecAddS(surf->tex->fuzz, R, tex_P, R);
            surf->tex->tex_fix(*surf, R, P);
        } else {
            surf->tex->tex_fix(*surf, tex_P, P);
        }
    }

    /* fix up any texture maps at the top level surface */
    if (surf->flags & S_TM_MAPPING) {
        surf->tex->map_fix(*surf, P); /* note that this get untranslated point */
    }

    //    VecMul(surf->diff, Ambient, col);    /* start with global ambient */
    //    col.r = surf->diff.r * Ambient.r;
    //    col.g = surf->diff.g * Ambient.g;
    //    col.b = surf->diff.b * Ambient.b;
    col = surf->diff * RayTrace_3D::ambient; /* start with global ambient */
                                //    VecAdd(surf->amb, col, col);        /* add self luminous */
    col += surf->amb;           /* add self luminous */

    /* make sure "real" normal is on right side */

    dot = VecDot(N, I);
    if (dot > 0.0) { /* if normal on wrong side, flip it */
        VecNegate(N);
        dot = -dot;
    }

    /* tweek surface normal, use L and R as temps */

    if (surf->bump) {
        if (surf->bump->turbulence) { /* thump that normal */
            VecCopy(tex_P, L);
            L[0] /= surf->bump->turbulence->scale[0];
            L[1] /= surf->bump->turbulence->scale[1];
            L[2] /= surf->bump->turbulence->scale[2];
            Noise::turb3(L, R, surf->bump->turbulence->terms);
            VecAddS(surf->bump->turbulence->amp, R, N, fuzz_N);
        } else {
            VecCopy(N, fuzz_N);
        }
        if (surf->bump->waves) { /* add waves */
            Wave_3D::make_waves(tex_P, R, surf->bump->waves);
            VecAdd(R, fuzz_N, fuzz_N);
        }
        VecNormalize(fuzz_N);
    } else {
        VecCopy(N, fuzz_N); /* no bump so just copy */
    }

    /* fuzz surface normal */

    if (surf->fuzz > 0.0) {
        double fuzziness;

        fuzziness = surf->fuzz;

        fuzz_N[0] += (bMath::rnd() - bMath::rnd()) * fuzziness;
        fuzz_N[1] += (bMath::rnd() - bMath::rnd()) * fuzziness;
        fuzz_N[2] += (bMath::rnd() - bMath::rnd()) * fuzziness;
        VecNormalize(fuzz_N);

        /* set flag for quick-fuzz mode */
        if (surf->flags & S_NO_ANTIALIAS && level == 0) {
            fuzzy_ray = 1;
        }
    }

    /* fold normal if needed */

    if (surf->bump || surf->fuzz > 0.0) { /* possible need to fold */
        /* first check for correct side of surface */
        dot = VecDot(N, fuzz_N);
        if (dot < 0.0) { /* need to fold */
            reflect(fuzz_N, N, fuzz_N, -dot);
        }
        /* now check to see if facing incident ray */
        dot = VecDot(I, fuzz_N);
        if (dot > 0.0) { /* need to fold */
            reflect(fuzz_N, I, fuzz_N, dot);
            dot = -dot;
        }
    } /* end of folding */

    dot = -dot; /* Make this positive for later use */

    R_calced = 0; /* no, we haven't calculated the reflection vector yet */

    /* Calculate diffuse component but only if we are entering
       a surface.  Checking for ior==1.0 is to catch goofs in
       the model such as having a goblet that is just a shell
       without any thickness. */

    if (inside == 0 || surf->flags & S_CACHE) { /* only if entering or cacheable */
        double light_spot; /* spot light brightness */
        cur_light = Light::light_head;
        for (cur_light = Light::light_head; cur_light; cur_light = cur_light->next) { /* for each light */
            /* get vector to light */
            switch (cur_light->type) {
            case L_DIRECTIONAL:
                VecCopy(cur_light->dir, L);
                break;
            case L_POINT:
                VecSub(cur_light->position, P, L);
                break;
            case L_SPHERICAL:
                VecSub(cur_light->position, P, L);
                break;
            case L_SPOT:
                VecSub(cur_light->position, P, L);
                VecCopy(L, tmpV);
                VecNormalize(tmpV);
                light_spot = VecDot(cur_light->dir, tmpV);
                if (light_spot < cur_light->max_angle) { /* if not in light */
                    continue;
                }
                if (light_spot >= cur_light->min_angle) { /* total illumination */
                    light_spot = 1.0;
                } else {
                    light_spot = (light_spot - cur_light->max_angle) / (cur_light->min_angle - cur_light->max_angle);
                }
                break;
            }
            if (VecDot(N, L) > 0.0 && VecDot(fuzz_N, L) > 0.0) { /* if not backfaced */
                t = VecNormalize(L);
                if (cur_light->type == L_DIRECTIONAL) {
                    t = HUGE_NUM;
                }
                VecCopy(P, tray.P);
                VecCopy(L, tray.D);
                // MakeVector(1.0, 1.0, 1.0, c_shadow);    /* needed to make no_shadow lights work */
                c_shadow = 1.0; /* needed to make no_shadow lights work */
                // memcpy((char *)(&nhit), (char *)hit, sizeof(Isect));
                nhit = hit;
                if (no_shadows
                || (cur_light->flag & L_NOSHADOWS)
                || (cur_light->type == L_SPHERICAL ? sShadow(&tray, nhit, t, c_shadow, level, *cur_light, inside) : shadow(&tray, nhit, t, c_shadow, level, *cur_light, inside))) {
                    double ldot;       /* normal to light angle */
                    ldot = VecDot(fuzz_N, L);
                    /* scale diff by angle of incidence */
                    // VecS(ldot, surf->diff, c_diff);
                    c_diff = (surf->diff * ldot);
                    /* scale light color by distance */
                    switch (cur_light->illum) {
                    case L_INFINITE:
                        // VecCopy(cur_light->color, c_dist);
                        c_dist = cur_light->color;
                        break;
                    case L_R:
                        // VecS(1.0/t, cur_light->color, c_dist);
                        c_dist = cur_light->color * (1.0 / t);
                        break;
                    case L_R_SQUARED:
                        // VecS(1.0/(t*t), cur_light->color, c_dist);
                        c_dist = cur_light->color * (1.0 / (t * t));
                        break;
                    }
                    /* mult by scaled light color */
                    // VecMul(c_dist, c_diff, c_diff);
                    c_diff *= c_dist;
                    /* modify if a spot light */
                    if (cur_light->type == L_SPOT) {
                        // VecS(light_spot, c_diff, c_diff);
                        c_diff *= light_spot;
                    }
                    /* mult by shadow color */
                    // VecMul(c_shadow, c_diff, c_diff);
                    c_diff *= c_shadow;
                    /* add to cumulative color */
                    // VecAdd(c_diff, col, col);
                    col += c_diff;

                    if (!inside && !(cur_light->flag & L_NOSPEC)) {
                        /* add specular spot */
                        if (!R_calced) {
                            reflect(I, fuzz_N, R, dot);
                            /* VecNormalize(R); */ /* already done if I and fuzz_N are normalized */
                            R_calced = 1;
                        }
                        if (surf->shine > Bob::rayeps) {
                            double spec = VecDot(R, L);
                            if (spec > Bob::rayeps) {
                                /* scale c_shadow(light color) by specular of surface or user input*/
                                // VecMul(surf->cshine, c_shadow, c_shadow);
                                c_shadow *= surf->cshine;
                                spec = pow(spec, surf->shine);
                                // VecAddS(spec, c_shadow, col, col);
                                col += (c_shadow * spec);
                            }
                        }
                    } /* end of light specular */
                }     /* end if shadowed */
            }
        } /* end of for each light loop */

        /* done with diffuse calculation */

        /* do specular reflection */

        VecCopy(P, tray.P);

        double max_weight = weight * bMath::max(bMath::max(surf->spec.r, surf->spec.g), surf->spec.b);
        if ((level < maxlevel - 1) && (max_weight > MINWEIGHT)) {
            nReflected++;
            if (R_calced) {
                VecCopy(R, tray.D);
            } else {
                reflect(I, fuzz_N, tray.D, dot);
            }
            trace(level + 1, max_weight, &tray, tcol, ior, hit.isect_self);
            // VecMul(surf->spec, tcol, tcol);
            tcol *= surf->spec;
            // VecAdd(tcol, col, col);
            col += tcol;
        }

    } else {                /* end of if entering */
        VecCopy(P, tray.P); /* needed for trans if not already done */
    }

    /* do transparency */

    if (level < maxlevel - 1 && (surf->trans.r > 0 || surf->trans.g > 0 || surf->trans.b > 0)) {
        int transmit = 0;
        int still_inside; /* local to save inside status */
        double new_ior; /* ior of material for transmitted ray */
        nRefracted++;
        if (inside == 0) { /* if entering */
            /* transmit = TransmissionDirection(NULL, surf, I, fuzz_N, tray.D, ior, dot); */
            transmit = refract(ior / surf->ior, I, fuzz_N, tray.D, dot);
        } else {
            if (level > maxlevel / 2) { /* time to bail */
                transmit = refract(DEFAULT_IOR / surf->ior, I, fuzz_N, tray.D, dot);
            } else {
                transmit = refract(surf->ior / DEFAULT_IOR, I, fuzz_N, tray.D, dot);
            }
        }
        if (transmit) {
            new_ior = hit.isect_enter ? (surf->ior) : DEFAULT_IOR;
            inside ^= 1; /* toggle inside flag */
            /* printf("level %d inside toggled to %d\n", level, inside); */
        } else { /* total internal reflection */
            new_ior = surf->ior;
            inside = 1; /* we're still inside */
            /* printf("level %d total internal reflection, inside at %d\n", level, inside); */
        }
        still_inside = inside;
        t = trace(level + 1, weight, &tray, tcol, new_ior, hit.isect_self);
        inside = still_inside;
        if (t < HUGE_NUM && inside) {
            // for(i=0; i<3; i++) {
            //     if(surf->trans[i] > Bob::rayeps) {
            //         if(surf->trans[i] < 1.0) {
            //             if(exp_trans) {
            //                 /* modulate color exponentially with distance */
            //                 /* ray traveled through substance */
            //                 /* if(i==0)
            //                     printf("level %d   inside %d   trans %.3f   t %.3f\n", level, inside, surf->trans[i], t); */
            //                 tcol[i] *= pow(surf->trans[i], t);
            //             } else {
            //                 tcol[i] *= surf->trans[i];
            //             }
            //         } // tcol[i] will remain the value returned by the call to Trace
            //     } else {
            //         tcol[i] = 0.0;  /* opaque */
            //     }
            // }

            /* modulate color exponentially with distance */
            /* ray traveled through substance */
            /* if(i==0)
                printf("level %d   inside %d   trans %.3f   t %.3f\n", level, inside, surf->trans.r, t); */
            if (surf->trans.r > Bob::rayeps) {
                if (surf->trans.r < 1.0) {
                    double etst = (exp_trans ? pow(surf->trans.r, t) : surf->trans.r);
                    tcol.r *= etst;
                } // tcol.r will remain the value returned by the call to Trace
            } else {
                tcol.r = 0.0; /* opaque */
            }
            if (surf->trans.g > Bob::rayeps) {
                if (surf->trans.g < 1.0) {
                    double etst = (exp_trans ? pow(surf->trans.g, t) : surf->trans.g);
                    tcol.g *= etst;
                } // tcol.g will remain the value returned by the call to Trace
            } else {
                tcol.g = 0.0; /* opaque */
            }
            if (surf->trans.b > Bob::rayeps) {
                if (surf->trans.b < 1.0) {
                    double etst = (exp_trans ? pow(surf->trans.b, t) : surf->trans.b);
                    tcol.b *= etst;
                } // tcol.b will remain the value returned by the call to Trace
            } else {
                tcol.b = 0.0; /* opaque */
            }
        }
        // VecAdd(tcol, col, col);
        col += tcol;
    }

    /* and finally add haze color if needed */

    if (inside == 0 && RayTrace_3D::hazeDensity > 0.0) {
        double haze;     /* % haze to add */
        haze = 1.0 - pow(1.0 - RayTrace_3D::hazeDensity, hit.isect_t);
        bkg(I, RayTrace_3D::hazeColor); /* get color for haze */
        // VecComb(haze, HazeColor, 1.0-haze, col, col);
        col = (RayTrace_3D::hazeColor * haze) + (col * (1.0 - haze));
    }

} /* end of shade() */
