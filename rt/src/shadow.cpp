/*
***************************************************************************
*                                                                         *
*                             Bob Ray Tracer                              *
*                                                                         *
*                    Shadow.C = shadow feeler routines                    *
*                                                                         *
*       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     *
*                                                                         *
*       ALL RIGHTS RESERVED.   This software is published, but is NOT     *
*         Public Domain and remains the propery of ALGORITHM, Inc.,       *
*   Christopher D. Watkins and Stephen B. Coy.  This software may not be  *
*  reproduced or integrated into other packages without the prior written *
*          consent of Christopher D. Watkins and Stephen B. Coy.          *
*                                                                         *
*                  Requires: defs.h, extern.h, config.h                   *
*                                                                         *
***************************************************************************

    For exp_trans true there will be problems with textured
    solid.  In particular the first surface encountered will
    not affect the shadow.  Only the outgoing surface will have
    any effect.  Because of this you must use no_exp_trans for
    any images using transparent textures.
*/

/*

    Shadow(ray, hit, tmax, color, level, light)

    ray -- which way to look
    hit -- object we hit if shadowed
    tmax -- distance to light
    color -- shadow color from transparent objects
    level -- current tree level, used for cache
    cur_light -- current light
    inside -- does shadow ray start inside an object?

    Returns 0 if totally shadowed
    Returns 1 if partially or fully unshadowed

    Shadow shoots a ray towards the light.  If no object is
    intersected the original point is considered unshadowed by
    that light source.  If an object is intersected that object's
    transparency is checked.  If the object is totally opaque,
    then the light is totally blocked.  Otherwise the object's
    transparent color and the distance the ray has to travel
    through the material are factored into the shadow color and
    the ray continues on from the intersection point to the light.
    It is assumed (for better or for worse) that the ray to the
    light source is starting in air, not in a transparent material.
*/
#include "RayTrace_3D.hpp"

#include <cstdio>
#include <cmath>
#include <memory.h>

#include "Bob.hpp"
#include "BobMath.hpp"
#include "Isect_3D.hpp"
#include "Object_3D.hpp"
#include "Surface_3D.hpp"
#include "defs.hpp"
#include "extern.hpp"
#include "config.hpp"


//extern int  Intersect (Ray *ray , Isect &hit , double maxdist , Object *self);
unsigned long RayTrace_3D::nShadows = 0;

//    Ray    *ray;
//    Isect    *hit;
//    double    tmax;        /* dist to light we are trying to hit */
//    Color    color;        /* for colored shadows */
//    int    level;        /* current tree level */
//    Light   *cur_light;     /* light we are checking for shadow */
//    int     inside;
int RayTrace_3D::shadow(Ray *ray, Isect &hit, double tmax, Color &color, int level, Light &cur_light, int inside) {
    Object            *cached;
    Surface_3D        *prev_surf;     /* used if light is inside object */
    static Surface_3D *tmp_surf;       /* to allow textured shadows */
    double             t, caustic_scale;
    int                in_surf;
    Point              P;              /* point for texture call */

    ++nShadows;
    prev_surf = NULL;
    in_surf = inside;

    /* check cache first */
    if((cached = cur_light.light_obj_cache[level])) {
        if((cached->intersect(ray, hit) && hit.isect_t < tmax)) {
            nShadowCacheHits++;
            return 0;
        }
    }
    cur_light.light_obj_cache[level] = NULL;    /* missed! */

    color = 1.0; /* start totally trans */

    while(RayTrace_3D::intersect(ray, hit, tmax, hit.isect_self)) {
        if(in_surf) {           /* if exiting */
            t = hit.isect_t;
            tmp_surf = hit.isect_surf;
            if(tmp_surf->tex) {      /* need to check texturing */
                RayPoint(*ray, t, P);
                tmp_surf->tex->tex_fix(*tmp_surf, P, P);
            } else if(tmp_surf->flags & S_TM_TRANS) {
                RayPoint(*ray, t, P);
                tmp_surf->tex->map_fix(*tmp_surf, P);
            }

            if(caustics) {
                Point   N;
                Object  *prim;

                RayPoint(*ray, t, P);
                prim = hit.isect_prim;
                prim->normal(hit, P, N);

                caustic_scale = VecDot(ray->D, N);
                if(caustic_scale < 0.0) {
                    caustic_scale = -caustic_scale;
                }
                caustic_scale = 0.8 + pow(caustic_scale, 25.0);
            } else {
                caustic_scale = 1.0;
            }

            if(tmp_surf->trans.r > Bob::rayeps) {
                double eTran = (exp_trans) ? pow(tmp_surf->trans.r, t) : tmp_surf->trans.r;
                color.r *= caustic_scale * eTran;
            } else {    /* its black so zero it out */
                color.r = 0.0;
            }
            if(tmp_surf->trans.g > Bob::rayeps) {
                double eTran = (exp_trans) ? pow(tmp_surf->trans.g, t) : tmp_surf->trans.g;
                color.g *= caustic_scale * eTran;
            } else {    /* its black so zero it out */
                color.g = 0.0;
            }
            if(tmp_surf->trans.b > Bob::rayeps) {
                double eTran = (exp_trans) ? pow(tmp_surf->trans.b, t) : tmp_surf->trans.b;
                color.b *= caustic_scale * eTran;
            } else {    /* its black so zero it out */
                color.b = 0.0;
            }

            /* check if totally blocked */
            if(color.r<Bob::rayeps && color.g<Bob::rayeps && color.b<Bob::rayeps) {
                if(tmp_surf->flags & S_CACHE) {  /* only if cacheable */
                    cur_light.light_obj_cache[level] = hit.isect_prim;
                }
                return 0;    /* totally shadowed */
            }
            in_surf = 0;
        } else {
            /* check for opaque object */
            t = hit.isect_t;
            tmp_surf = hit.isect_surf;
            if(tmp_surf->tex) {      /* need to check texturing */
                RayPoint(*ray, t, P);
                tmp_surf->tex->tex_fix(*tmp_surf, P, P);
            } else if(tmp_surf->flags & S_TM_TRANS) {
                RayPoint(*ray, t, P);
                tmp_surf->tex->map_fix(*tmp_surf, P);
            }
            if(tmp_surf->trans.r < Bob::rayeps &&
               tmp_surf->trans.g < Bob::rayeps &&
               tmp_surf->trans.b < Bob::rayeps) {
                color = 0.0;
                if(tmp_surf->flags & S_CACHE) {  /* only if cacheable */
                    cur_light.light_obj_cache[level] = hit.isect_prim;
                }
                return 0;
            }

            /* if not totally blocked, toggle inside flag and move on */
            in_surf = 1;
            prev_surf = hit.isect_surf;
        }

        /* make intersection point new start point and continue */
        RayPoint(*ray, hit.isect_t, ray->P);
        tmax -= hit.isect_t;
    }

    if(in_surf && prev_surf) {      /* if light is in transparent object */
            if(tmp_surf->trans.r > Bob::rayeps) {
                double eTran = (exp_trans) ? pow(tmp_surf->trans.r, tmax) : tmp_surf->trans.r;
                color.r *= eTran;
            } else {    /* its black so zero it out */
                color.r = 0.0;
            }
            if(tmp_surf->trans.g > Bob::rayeps) {
                double eTran = (exp_trans) ? pow(tmp_surf->trans.g, tmax) : tmp_surf->trans.g;
                color.g *= eTran;
            } else {    /* its black so zero it out */
                color.g = 0.0;
            }
            if(tmp_surf->trans.b > Bob::rayeps) {
                double eTran = (exp_trans) ? pow(tmp_surf->trans.b, tmax) : tmp_surf->trans.b;
                color.b *= eTran;
            } else {    /* its black so zero it out */
                color.b = 0.0;
            }
    }

    return 1;    /* fully or partially illuminated */
}

/*

    sShadow(ray, hit, tmax, color, level, light, radius, inside)

    sShadow() provides a 'shell' around Shadow for dealing with
    spherical light sources.  sShadow calls Shadow l_samples
    times.  Each time the ray is tweeked to hit somewhere on the
    surface of the light being sampled.
*/

//TODO: TCE:Should fall under Ray?
//    Ray    *ray;
//    Isect    *hit;
//    double    tmax;        /* dist to light we are trying to hit */
//    Color    color;        /* for colored shadows */
//    int    level;        /* current tree level */
//    Light   *cur_light;     /* light source */
//    int     inside;
int RayTrace_3D::sShadow(Ray *ray, Isect &hit, double tmax, Color &color, int level, Light &cur_light, int inside) {
    int   sample, visible;
    Color tmp_color;
    Ray   tmp_ray;
    Vec   tweek;
    double   radius;        /* light radius */
    double   len;        /* length to tweek vector */

    radius = cur_light.radius;
    color = 0.0;
    visible = 0;        /* assume totally shadowed to start */
    len = radius/tmax;
    for(sample=0; sample<cur_light.samples; sample++) {
        VecCopy(ray->D, tmp_ray.D);    /* use tmp ray */
        VecCopy(ray->P, tmp_ray.P);

        /* tweek ray direction */
        MakeVector(0.5-bMath::rnd(), 0.5-bMath::rnd(), 0.5-bMath::rnd(), tweek);
        VecNormalize(tweek);
        VecAddS(len, tweek, tmp_ray.D, tmp_ray.D);
        VecNormalize(tmp_ray.D);

        if(shadow(&tmp_ray, hit, tmax, tmp_color, level, cur_light, inside)) {
            color += tmp_color;
            visible = 1;
        }
    }
    double invcls = (1.0/cur_light.samples);
    color *= invcls;

    return visible;
}
