/*
�������������������������������������������������������������������������ͻ
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�       Shade.C = Given the intersection point shade() calculates the     �
�		  actual color at that point.  This may involve           �
�	       	  extra calls to trace for reflection and refraction.     �
�                                                                         �
�       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     �
�                                                                         �
�       ALL RIGHTS RESERVED.   This software is published, but is NOT     �
�         Public Domain and remains the propery of ALGORITHM, Inc.,       �
�   Christopher D. Watkins and Stephen B. Coy.  This software may not be  �
�  reproduced or integrated into other packages without the prior written �
�          consent of Christopher D. Watkins and Stephen B. Coy.          �
�                                                                         �
�                  Requires: defs.h, extern.h,c onfig.h                   �
�                                                                         �
�������������������������������������������������������������������������ͼ
*/

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <memory.h>
#include "defs.hpp"
#include "extern.hpp"
#include "config.hpp"

#define DEFAULT_IOR     (1.0)

#define SIGN(a) ((a)>0?1:((a)<0?(-1):0))

/*
	Shade(level, weight, P, N, I, hit, col, ior)

	level	current depth in shading tree
	weight	used for early cut off
	P	intersection point
	N       surface normal at intersection
	I	incident vector, points along ray toward intersection
	hit	what we hit
	col	color to return
	ior	current ior
*/

void	Shade(int level, Flt weight, Vec P, Vec N, Vec I, Isect *hit, Color col, Flt ior)
{
	/* the following locals have been declared static to help */
	/* shrink the amount of stack spaced needed for recursive */
	/* calls to shade() */

	static int      inside; /* new inside/outside determination */
	Vec
		tex_P,          /* transformed point for textures */
		L, R,
		tmpV;           /* direction to light vector */
	Vec     fuzz_N;         /* fuzzed, bumped and waved surface normal */
	static Flt
		haze;           /* % haze to add */
	static Color
		c_diff,         /* diffuse color */
		c_dist,         /* light color scaled by distance */
		c_shadow;       /* shadow color */
	static Flt
		spec;
	int
		i, dot_sign,
		R_calced,	/* Has R already be calculated? */
		transmit;
	Flt
		light_spot,     /* spot light brightness */
		dot,		/* cos of incident angle */
		ldot,		/* normal to light angle */
		max_weight;
	static Light
		*cur_light;     /* ptr to current light we're looking at */

	Isect   nhit;
	Ray     tray;
	Color 	tcol;
	Flt     t;
	Flt     new_ior;        /* ior of material for transmitted ray */
	Surface *surf;
	int     still_inside;   /* local to save inside status */

	if(level == 0) {        /* assume all eye rays start outside */
		inside = 0;
	}

	surf = hit->isect_prim->o_surf; /* pick out surf we hit */

	/* transform point if needed */

	if(surf->flags & S_TRANSFORM) {
		trans_vector(surf->matrix, P, tex_P);
	} else {
		VecCopy(P, tex_P);
	}

	/* handle any texturing up front */

	if(surf->tex) {
		if(surf->tex->fuzz > 0.0) {
			MakeVector(rnd(), rnd(), rnd(), R);
			if(rnd() > 0.5) R[0] = -R[0];
			if(rnd() > 0.5) R[1] = -R[1];
			if(rnd() > 0.5) R[2] = -R[2];
			VecAddS(surf->tex->fuzz, R, tex_P, R);
			tex_fix(surf, R, P);
		} else {
			tex_fix(surf, tex_P, P);
		}
	}

	/* fix up any texture maps at the top level surface */
	if(surf->flags & S_TM_MAPPING) {
		map_fix(surf, P);       /* note that this get untranslated point */
	}

	VecMul(surf->diff, Ambient, col);	/* start with global ambient */
	VecAdd(surf->amb, col, col);		/* add self luminous */

	/* make sure "real" normal is on right side */

	dot = VecDot(N, I);
	if(dot > 0.0) {         /* if normal on wrong side, flip it */
		VecNegate(N);
		dot = -dot;
	}

	/* tweek surface normal, use L and R as temps */

	if(surf->bump) {
		if(surf->bump->turbulence) {    /* thump that normal */
			VecCopy(tex_P, L);
			L[0] /= surf->bump->turbulence->scale[0];
			L[1] /= surf->bump->turbulence->scale[1];
			L[2] /= surf->bump->turbulence->scale[2];
			turb3(L, R, surf->bump->turbulence->terms);
			VecAddS(surf->bump->turbulence->amp, R, N, fuzz_N);
		} else {
			VecCopy(N, fuzz_N);
		}
		if(surf->bump->waves) {        /* add waves */
			make_waves(tex_P, R, surf->bump->waves);
			VecAdd(R, fuzz_N, fuzz_N);
		}
		VecNormalize(fuzz_N);
	} else {
		VecCopy(N, fuzz_N);     /* no bump so just copy */
	}

	/* fuzz surface normal */

	if(surf->fuzz > 0.0) {
		Flt     fuzziness;

		fuzziness = surf->fuzz;
		fuzz_N[0] += (rnd() - rnd())*fuzziness;
		fuzz_N[1] += (rnd() - rnd())*fuzziness;
		fuzz_N[2] += (rnd() - rnd())*fuzziness;
		VecNormalize(fuzz_N);

		/* set flag for quick-fuzz mode */
		if(surf->flags & S_NO_ANTIALIAS && level==0) {
			fuzzy_ray = 1;
		}
	}

	/* fold normal if needed */

	if(surf->bump || surf->fuzz > 0.0) {    /* possible need to fold */
		/* first check for correct side of surface */
		dot = VecDot(N, fuzz_N);
		if(dot < 0.0) {         /* need to fold */
			reflect(fuzz_N, N, fuzz_N, -dot);
		}
		/* now check to see if facing incident ray */
		dot = VecDot(I, fuzz_N);
		if(dot > 0.0) {         /* need to fold */
			reflect(fuzz_N, I, fuzz_N, dot);
			dot = -dot;
		}
	}       /* end of folding */

	dot = -dot;     /* Make this positive for later use */

	R_calced = 0;   /* no, we haven't calculated the reflection vector yet */

	/* Calculate diffuse component but only if we are entering 
	   a surface.  Checking for ior==1.0 is to catch goofs in
	   the model such as having a goblet that is just a shell
	   without any thickness. */

	if(inside == 0 || surf->flags & S_CACHE) {      /* only if entering or cacheable */
		cur_light = light_head;
		for(cur_light = light_head; cur_light; cur_light = cur_light->next) {            /* for each light */
			/* get vector to light */
			switch(cur_light->type) {
				case L_DIRECTIONAL :
					VecCopy(cur_light->dir, L);
					break;
				case L_POINT :
					VecSub(cur_light->position, P, L);
					break;
				case L_SPHERICAL :
					VecSub(cur_light->position, P, L);
					break;
				case L_SPOT :
					VecSub(cur_light->position, P, L);
					VecCopy(L, tmpV);
					VecNormalize(tmpV);
					light_spot = VecDot(cur_light->dir, tmpV);
					if(light_spot < cur_light->max_angle) {         /* if not in light */
						continue;
					}
					if(light_spot >= cur_light->min_angle) {        /* total illumination */
						light_spot = 1.0;
					} else {
						light_spot = (light_spot - cur_light->max_angle) /
							(cur_light->min_angle - cur_light->max_angle);
					}
					break;
			}
			if(VecDot(N, L)>0.0 && VecDot(fuzz_N, L)>0.0) { /* if not backfaced */
				t = VecNormalize(L);
				if(cur_light->type == L_DIRECTIONAL) {
					t = HUGE;
				}
				VecCopy(P, tray.P);
				VecCopy(L, tray.D);
				MakeVector(1.0, 1.0, 1.0, c_shadow);    /* needed to make no_shadow lights work */
				memcpy((char *)(&nhit), (char *)hit, sizeof(Isect));
				if(no_shadows || (cur_light->flag & L_NOSHADOWS) || (cur_light->type == L_SPHERICAL ?
					sShadow(&tray, &nhit, t, c_shadow, level, cur_light, inside) :
					Shadow(&tray, &nhit, t, c_shadow, level, cur_light, inside))) {
					ldot = VecDot(fuzz_N, L);
					/* scale diff by angle of incidence */
					VecS(ldot, surf->diff, c_diff);
					/* scale light color by distance */
					switch(cur_light->illum) {
						case L_INFINITE :
							VecCopy(cur_light->color, c_dist);
							break;
						case L_R :
							VecS(1.0/t, cur_light->color, c_dist);
							break;
						case L_R_SQUARED :
							VecS(1.0/(t*t), cur_light->color, c_dist);
							break;
					}
					/* mult by scaled light color */
					VecMul(c_dist, c_diff, c_diff);
					/* modify if a spot light */
					if(cur_light->type == L_SPOT) {
						VecS(light_spot, c_diff, c_diff);
					}
					/* mult by shadow color */
					VecMul(c_shadow, c_diff, c_diff);
					/* add to cumulative color */
					VecAdd(c_diff, col, col);

					if(!inside && !(cur_light->flag & L_NOSPEC)) {
						/* add specular spot */
						if(!R_calced) {
							reflect(I, fuzz_N, R, dot);
							/* VecNormalize(R); */ /* already done if I and fuzz_N are normalized */
							R_calced = 1;
						}
						if (surf->shine > rayeps) {
							spec = VecDot(R, L);
							if (spec > rayeps) {
								/* scale c_shadow(light color) by specular of surface or user input*/
								VecMul(surf->cshine, c_shadow, c_shadow);
								spec = pow(spec, surf->shine);
								VecAddS(spec, c_shadow, col, col);
							}
						}
					}       /* end of light specular */
				}	/* end if shadowed */
			}
		}	/* end of for each light loop */

		/* done with diffuse calculation */

		/* do specular reflection */

		VecCopy(P, tray.P);

		max_weight = weight * MAX(MAX(surf->spec[0], surf->spec[1]), surf->spec[2]);
		if((level < maxlevel-1) && (max_weight > minweight)) {
			nReflected++;
			if(R_calced) {
				VecCopy(R, tray.D);
			} else {
				reflect(I, fuzz_N, tray.D, dot);
			}
			t = Trace(level+1, max_weight, &tray, tcol, ior, hit->isect_self);
			VecMul(surf->spec, tcol, tcol);
			VecAdd(tcol, col, col);
		}

	} else {        /* end of if entering */
		VecCopy(P, tray.P);     /* needed for trans if not already done */
	}

	/* do transparency */

	if(level < maxlevel-1 && (surf->trans[0]>0 || surf->trans[1]>0 || surf->trans[2]>0)) {
		nRefracted++;
		if(inside == 0) {  /* if entering */
			/* transmit = TransmissionDirection(NULL, surf, I, fuzz_N, tray.D, ior, dot); */
			transmit = refract(ior/surf->ior, I, fuzz_N, tray.D, dot);
		} else {
			if(level > maxlevel/2) {        /* time to bail */
				transmit = refract(DEFAULT_IOR/surf->ior, I, fuzz_N, tray.D, dot);
			} else {
				transmit = refract(surf->ior/DEFAULT_IOR, I, fuzz_N, tray.D, dot);
			}
		}
		if(transmit) {
			new_ior = hit->isect_enter ? (surf->ior) : DEFAULT_IOR;
			inside ^= 1;    /* toggle inside flag */
			/* printf("level %d inside toggled to %d\n", level, inside); */
		} else {        /* total internal reflection */
			new_ior = surf->ior;
			inside = 1;     /* we're still inside */
			/* printf("level %d total internal reflection, inside at %d\n", level, inside); */
		}
		still_inside = inside;
		t = Trace(level+1, weight, &tray, tcol, new_ior, hit->isect_self);
		inside = still_inside;
		if(t<HUGE && inside) {
			for(i=0; i<3; i++) {
				if(surf->trans[i] > rayeps) {
					if(surf->trans[i] < 1.0) {
						if(exp_trans) {
							/* modulate color exponentially with distance */
							/* ray traveled through substance */
							/* if(i==0)
								printf("level %d   inside %d   trans %.3f   t %.3f\n", level, inside, surf->trans[i], t); */
							tcol[i] *= pow(surf->trans[i], t);
						} else {
							tcol[i] *= surf->trans[i];
						}
					}
				} else {
					tcol[i] = 0.0;  /* opaque */
				}
			}
		}
		VecAdd(tcol, col, col);
	}

	/* and finally add haze color if needed */

	if(inside == 0 && HazeDensity > 0.0) {
		haze = 1.0 - pow(1.0-HazeDensity, hit->isect_t);
		bkg(I, HazeColor);      /* get color for haze */
		VecComb(haze, HazeColor, 1.0-haze, col, col);
	}

}       /* end of shade() */

/*
	reflect -- given an incident vector I, and the normal N,
		calculate the direction of the reflected ray R.
*/

void    reflect(Vec I, Vec N, Vec R, Flt dot)
//	Vec	I, N, R;
//	Flt	dot;		/* -VecDot(I,N) */
{
	Flt     len;

	len = 2.0 * dot;
	VecAddS(len, N, I, R);
}

/*
	refract(ior_ratio, I, N, T) calculates direction of refracted
		angle, T.  Returns 0 if total internal reflection occurs,
		1 otherwise.
*/

int     refract(Flt eta, Vec I, Vec N, Vec T, Flt dot)
//	Flt     eta;    /* ratio of old/new iors */
//	Vec     I,      /* incident vector */
//		N,      /* surface normal */
//		T;      /* transmitted vector (calculated) */
//	Flt     dot;    /* -VecDot(I, N) */
{
	Flt     n1, n2,
		c1, cs2;

	if(eta == 1.0) {        /* bail out early */
		VecCopy(I, T);
		return 1;
	}

	c1 = dot;
	cs2 = 1.0 - eta*eta*(1.0 - c1*c1);
	if(cs2 < 0.0) {
		reflect(I, N, T, dot);
		return 0;       /* total internal reflection */
	}
	cs2 = eta*c1 - sqrt(cs2);
	VecComb(eta, I, cs2, N, T);

	return 1;
}

