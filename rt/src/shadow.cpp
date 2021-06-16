/*
�������������������������������������������
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�                    Shadow.C = shadow feeler routines                    �
�                                                                         �
�       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     �
�                                                                         �
�       ALL RIGHTS RESERVED.   This software is published, but is NOT     �
�         Public Domain and remains the propery of ALGORITHM, Inc.,       �
�   Christopher D. Watkins and Stephen B. Coy.  This software may not be  �
�  reproduced or integrated into other packages without the prior written �
�          consent of Christopher D. Watkins and Stephen B. Coy.          �
�                                                                         �
�                  Requires: defs.h, extern.h, config.h                   �
�                                                                         �
�������������������������������������������

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

#include <cstdio>
#include <cmath>
#include <memory.h>
#include "defs.hpp"
#include "extern.hpp"
#include "config.hpp"
#include "proto.hpp"

int     Shadow(Ray *ray, Isect *hit, Flt tmax, Color color, int level, Light *cur_light, int inside)
//    Ray    *ray;
//    Isect    *hit;
//    Flt    tmax;        /* dist to light we are trying to hit */
//    Color    color;        /* for colored shadows */
//    int    level;        /* current tree level */
//    Light   *cur_light;     /* light we are checking for shadow */
//    int     inside;
{
    Object          *cached;
    Surface         *prev_surf;     /* used if light is inside object */
    static Surface  tmp_surf;       /* to allow textured shadows */
    static int      i, in_surf;
    Flt             t, caustic_scale;
    Vec             P;              /* point for texture call */

    ++nShadows;
    prev_surf = NULL;
    in_surf = inside;

    /* check cache first */
    if(cached = cur_light->light_obj_cache[level]) {
        if((cached->o_procs->intersect)(cached, ray, hit)
            && hit->isect_t < tmax) {
            nShadowCacheHits++;
            return 0;
        }
    }
    cur_light->light_obj_cache[level] = NULL;    /* missed! */

    MakeVector(1.0, 1.0, 1.0, color);       /* start totally trans */

    while(Intersect(ray, hit, tmax, hit->isect_self)) {
        if(in_surf) {           /* if exiting */
            t = hit->isect_t;
            memcpy(&tmp_surf, hit->isect_surf, sizeof(Surface));
            if(tmp_surf.tex) {      /* need to check texturing */
                RayPoint(ray, t, P);
                tex_fix(&tmp_surf, P, P);
            } else if(tmp_surf.flags & S_TM_TRANS) {
                RayPoint(ray, t, P);
                map_fix(&tmp_surf, P);
            }

            if(caustics) {
                Point   N;
                Object  *prim;

                RayPoint(ray, t, P);
                prim = hit->isect_prim;
                /* (*prim->o_procs->normal) (prim, &hit, P, N); */
                (*prim->o_procs->normal) (prim, hit, P, N);

                caustic_scale = VecDot(ray->D, N);
                if(caustic_scale < 0.0) {
                    caustic_scale = -caustic_scale;
                }
                caustic_scale = 0.8 + pow(caustic_scale, 25.0);
            } else {
                caustic_scale = 1.0;
            }

            for(i=0; i<3; i++) {
                if(tmp_surf.trans[i] > rayeps) {
                    if(exp_trans) {
                        color[i] *= caustic_scale * pow(tmp_surf.trans[i], t);
                    } else {
                        color[i] *= caustic_scale * tmp_surf.trans[i];
                    }
                } else {    /* its black so zero it out */
                    color[i] = 0.0;
                }
            }
            /* check if totally blocked */
            if(color[0]<rayeps && color[1]<rayeps && color[2]<rayeps) {
                if(tmp_surf.flags & S_CACHE) {  /* only if cacheable */
                    cur_light->light_obj_cache[level] = hit->isect_prim;
                }
                return 0;    /* totally shadowed */
            }
            in_surf = 0;
        } else {
            /* check for opaque object */
            t = hit->isect_t;
            memcpy(&tmp_surf, hit->isect_surf, sizeof(Surface));
            if(tmp_surf.tex) {      /* need to check texturing */
                RayPoint(ray, t, P);
                tex_fix(&tmp_surf, P, P);
            } else if(tmp_surf.flags & S_TM_TRANS) {
                RayPoint(ray, t, P);
                map_fix(&tmp_surf, P);
            }
            if(tmp_surf.trans[0] < rayeps &&
               tmp_surf.trans[1] < rayeps &&
               tmp_surf.trans[2] < rayeps) {
                MakeVector(0.0, 0.0, 0.0, color);
                if(tmp_surf.flags & S_CACHE) {  /* only if cacheable */
                    cur_light->light_obj_cache[level] = hit->isect_prim;
                }
                return 0;
            }

            /* if not totally blocked, toggle inside flag and move on */
            in_surf = 1;
            prev_surf = hit->isect_surf;
        }

        /* make intersection point new start point and continue */
        RayPoint(ray, hit->isect_t, ray->P);
        tmax -= hit->isect_t;
    }

    if(in_surf && prev_surf) {      /* if light is in transparent object */
        for(i=0; i<3; i++) {
            if(tmp_surf.trans[i] > rayeps) {
                if(exp_trans) {
                    color[i] *= pow(tmp_surf.trans[i], tmax);
                } else {
                    color[i] *= tmp_surf.trans[i];
                }
            } else {
                color[i] = 0.0;
            }
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

int     sShadow(Ray *ray, Isect *hit, Flt tmax, Color color, int level, Light *cur_light, int inside)
//    Ray    *ray;
//    Isect    *hit;
//    Flt    tmax;        /* dist to light we are trying to hit */
//    Color    color;        /* for colored shadows */
//    int    level;        /* current tree level */
//    Light   *cur_light;     /* light source */
//    int     inside;
{
    int    sample, visible;
    Color    tmp_color;
    Ray    tmp_ray;
    Vec    tweek;
    Flt    radius;        /* light radius */
    Flt    len;        /* length to tweek vector */

    radius = cur_light->radius;
    MakeVector(0.0, 0.0, 0.0, color);
    visible = 0;        /* assume totally shadowed to start */
    len = radius/tmax;
    for(sample=0; sample<cur_light->samples; sample++) {
        VecCopy(ray->D, tmp_ray.D);    /* use tmp ray */
        VecCopy(ray->P, tmp_ray.P);

        /* tweek ray direction */
        MakeVector(0.5-rnd(), 0.5-rnd(), 0.5-rnd(), tweek);
        VecNormalize(tweek);
        VecAddS(len, tweek, tmp_ray.D, tmp_ray.D);
        VecNormalize(tmp_ray.D);

        if(Shadow(&tmp_ray, hit, tmax, tmp_color, level, cur_light, inside)) {
            VecAdd(tmp_color, color, color);
            visible = 1;
        }
    }
    VecS((1.0/cur_light->samples), color, color);

    return visible;
}

