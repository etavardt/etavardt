/*
�������������������������������������������������������������������������ͻ
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�         Trace.C = Where the rays are actually shot out into the         �
�		word with grand hopes of hitting a primitive and          �
�		adding color to the scene.  Here we also deal with        �
�		those poor rays whose lot in life is to never hit         �
�		anything, proceding out into never-never land.            �
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
�������������������������������������������������������������������������ͼ

	Trace returns the dist to the next hit.  This gives shade()
	a value to use for determining color attenuation for haze and
	transparent objects.
*/

#include <cstdio>
#include <cmath>
#include "defs.hpp"
#include "extern.hpp"


void    bkg(Vec dir, Color col)
{
	Flt     dot, index;
	int     indx;

	if(background.color[0] < 0.0) {
		dot = -VecDot(dir, background.up);
		index = 127.0 * dot + 128.0;
		indx = index;
		index -= indx;
		col[0] = (1.0-index)*background.pal[indx][0]/256.0 + index*background.pal[indx+1][0]/256.0;
		col[1] = (1.0-index)*background.pal[indx][1]/256.0 + index*background.pal[indx+1][1]/256.0;
		col[2] = (1.0-index)*background.pal[indx][2]/256.0 + index*background.pal[indx+1][2]/256.0;
	} else {
		VecCopy(background.color, col);
	}
}       /* end of bkg */

Flt     Trace(int	level, Flt	weight, Ray	*ray, Color	color, Flt	ior, Object	*self)
//	Flt	ior;		/* current material ior */
{
	Object	*prim;
	Vec	P, N;
	Isect	hit;

	if(level >= maxlevel) {
		color[0] = color[1] = color[2] = 0.0;
		return 0.0;
	} else if(level > deepest) {
		deepest = level;
	}
		
	nRays ++;

	if(Intersect(ray, &hit, HUGE, self)) {

		/* end of warning */

		prim = hit.isect_prim;
		RayPoint(ray, hit.isect_t, P);
		/* get normal vector of intersection */
		(*prim -> o_procs -> normal) (prim, &hit, P, N);

		Shade(level, weight, P, N, ray->D, &hit, color, ior);
		return hit.isect_t;
	} else {
		bkg(ray->D, color);

		return HUGE;
	}
}

