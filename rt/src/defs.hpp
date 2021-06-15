/*
�������������������������������������������������������������������������ͻ
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�                    Defs.C = the center of all confusion                 �
�                                                                         �
�       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     �
�                                                                         �
�       ALL RIGHTS RESERVED.   This software is published, but is NOT     �
�         Public Domain and remains the propery of ALGORITHM, Inc.,       �
�   Christopher D. Watkins and Stephen B. Coy.  This software may not be  �
�  reproduced or integrated into other packages without the prior written �
�          consent of Christopher D. Watkins and Stephen B. Coy.          �
�                                                                         �
�                          Requires: config.h                             �
�                                                                         �
�������������������������������������������������������������������������ͼ
*/

#include "config.h"

#ifdef	HUGE
#undef	HUGE
#endif

#define HUGE    (1.0e8)

/*
	generic helpful macros
*/

#define ABS(a)          (((a)<0)?(-(a)):(a))
#define FLOOR(a)	((a)>0?(int)(a):-(int)(a))
#define CEILING(a)	((a)==(int)(a)?(a):(a)>0?1+(int)(a):-(1+(int)(-a))
#define ROUND(a)	((a)>0?(int)((a)+0.5):-(int)(0.5-a))

#define MIN(a,b)	(((a)<(b))?(a):(b))
#define MAX(a,b)        (((a)>(b))?(a):(b))

/*
	typedefs for the world
*/

typedef double	Flt;
typedef Flt	Vec[3];
typedef Vec	Point;
typedef Vec	Color;
typedef Flt	Matrix[4][4];

/*----------------------------------------------------------------------*/

#ifndef DUMB_CPP 

#define MakeVector(x, y, z, v)		(v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecNegate(a)	(a)[0]=(-(a)[0]);\
			(a)[1]=(-(a)[1]);\
			(a)[2]=(-(a)[2]);
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecLen(a)	(sqrt(VecDot(a,a)))
#define VecCopy(a,b)	(b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2];
#define VecAdd(a,b,c)	(c)[0]=(a)[0]+(b)[0];\
			(c)[1]=(a)[1]+(b)[1];\
			(c)[2]=(a)[2]+(b)[2]
#define VecSub(a,b,c)	(c)[0]=(a)[0]-(b)[0];\
			(c)[1]=(a)[1]-(b)[1];\
			(c)[2]=(a)[2]-(b)[2]
#define VecComb(A,a,B,b,c)	(c)[0]=(A)*(a)[0]+(B)*(b)[0];\
				(c)[1]=(A)*(a)[1]+(B)*(b)[1];\
			 	(c)[2]=(A)*(a)[2]+(B)*(b)[2]
#define VecS(A,a,b)		(b)[0]=(A)*(a)[0];\
				(b)[1]=(A)*(a)[1];\
				(b)[2]=(A)*(a)[2]
#define VecAddS(A,a,b,c)	(c)[0]=(A)*(a)[0]+(b)[0];\
				(c)[1]=(A)*(a)[1]+(b)[1];\
				(c)[2]=(A)*(a)[2]+(b)[2]
#define VecMul(a,b,c)		(c)[0]=(a)[0]*(b)[0];\
				(c)[1]=(a)[1]*(b)[1];\
				(c)[2]=(a)[2]*(b)[2]
#define VecCross(a,b,c)		(c)[0]=(a)[1]*(b)[2]-(a)[2]*(b)[1];\
				(c)[1]=(a)[2]*(b)[0]-(a)[0]*(b)[2];\
				(c)[2]=(a)[0]*(b)[1]-(a)[1]*(b)[0]
#define VecZero(v)		(v)[0]=0.0;(v)[1]=0.0;v[2]=0.0
#define VecPrint(msg,v)		fprintf(stderr, "%s: %g %g %g\n", msg,\
					(v)[0],(v)[1],(v)[2])

#endif /* not DUMB_CPP */

/*----------------------------------------------------------------------*/

typedef struct Ray {
	Point P;
	Point D;
} Ray;

#define RayPoint(ray,t,point)	VecAddS(t,(ray)->D,(ray)->P,point)

/*
	texture map structure
*/

typedef struct t_texmap {
	Vec     position,       /* upper left hand corner of image */
		normal,         /* same as projection direction */
		across,         /* across top of image to upper right */
		down;           /* down to lower left */
	Flt     scale;          /* defaults to 1.0, #units across full image */
	int     xres, yres;     /* image size */
	unsigned char
		**red,
		**grn,
		**blu;
} Texmap;

/* surface structure */

typedef struct t_surface {
	Color   diff;                   /* diffuse */
	Texmap  *tm_diff;
	Color	spec;			/* specular (reflected) */
	Texmap  *tm_spec;
	Color	amb;			/* ambient */
	Texmap  *tm_amb;
	Flt     shine;                  /* specular spot exponent */
	Color   cshine;                 /* spec spot color */
	Color   trans;                  /* transparency */
	Texmap  *tm_trans;
	Flt	ior;			/* index of refraction */
	Flt     fuzz;                   /* surface fuzz */
	int     flags;                  /* is this surface valid for shadow caching */
	struct t_texture	*tex;	/* ptr for color texture */
	struct t_bump           *bump;  /* ptr for surface normal texture */
	Matrix  matrix;                 /* transformation matrix */
} Surface;

/* surface flags */

#define S_CACHE         (0x0001)
#define S_NO_ANTIALIAS  (0x0002)
#define S_TRANSFORM     (0x0004)

#define S_TM_DIFF       (0x0008)
#define S_TM_SPEC       (0x0010)
#define S_TM_TRANS      (0x0020)
#define S_TM_AMB        (0x0040)
#define S_TM_MAPPING    (0x0078)        /* all bits for mapping */

typedef struct t_turbulence {
	int     terms;          /* # of terms in the series */
	Vec     trans,          /* pre-process numbers */
		scale;
	Flt     amp;            /* post turbulence amplification */
} Turbulence;

typedef struct t_wave {
	Vec	center;
	Flt	wavelength,
		amp,		/* should be about 0 to .6 */
		damp,		/* damping per wavelength */
		phase;		/* wavelength offset */
	struct t_wave	*next;	/* next wave in line */
} Wave;

typedef struct t_texture {
	Flt	(*func)(Point, t_texture*);	/* returns 0.0..1.0 */
	Flt	blur;		/* % blending between layers */
	Flt	fuzz;		/* white noise blending */
	Flt     r1, r2;         /* just because */
	int     terms;          /* for tex_noise() */
	Vec     trans,
		scale;
	Surface *surf[2];
	Turbulence      *turbulence;
	Wave	*waves;
} Texture;

typedef struct t_bump {
	Vec	trans,		/* apply to resulting normal */
		scale;
	Turbulence      *turbulence;
	Wave            *waves;
} Bump;

typedef struct t_infile {
	char            *file_name;
	FILE            *fp;
	long            line;
	struct t_infile *next;
} Infile;

typedef struct t_stack {
	Infile		*what;
	struct t_stack	*prev;
} Stack;

/*	light type constants */

#define L_DIRECTIONAL	(1)
#define L_POINT		(2)
#define L_SPHERICAL	(3)
#define L_SPOT		(4)

/*      light illumination constants */

#define L_INFINITE	(0)
#define L_R		(1)
#define L_R_SQUARED	(2)

/*      misc light flags */

#define L_NOSHADOWS     (1)
#define L_NOSPEC        (2)

typedef struct t_light {
	Vec	position;
	Vec	dir;
	Color	color;
	Flt     radius;         /* radius/spherical, max_angle/spotlight */
	Flt     min_angle,      /* angles for spot lights */
		max_angle;
	short	type;		/* what type is this? */
	short	illum;		/* how does the light fall off? */
	short   flag;           /* noshadows? nospec? */
	short   samples;        /* num samples for spherical light */
	struct t_object *light_obj_cache[MAXLEVEL];
	struct t_light  *next;  /* next light in list */
} Light;

typedef struct t_viewpoint {
	Vec	view_from;
	Vec	view_at;
	Vec	view_up;
	Flt	view_angle_x;
	Flt	view_angle_y;
	Flt	view_aspect;
} Viewpoint;

typedef struct t_camera {
	short   projection;
	Vec	lens_i,		/* vectors across lens */
		lens_j;
	Flt	aperture,	/* radius of lens */
		focal_length;   /* how far away are we focussed */
	short   samples;        /* num samples for non-pinhole camera */
} Camera;

/*
	Clipping
*/

#define C_PLANE         (0x01)
#define C_SPHERE        (0x02)
#define C_CONE          (0x04)
#define C_INSIDE        (0x10)

typedef struct t_clip {
	Vec     center,
		normal,
		apex, base;
	Flt     radius1, radius2, length;
	int     type;
	struct t_clip   *next;
} Clip;

typedef struct t_global_clip {
	Clip                    *clip;
	struct t_global_clip    *next;
} GlobalClip;

typedef t_object Object;

typedef struct t_isect {
	Flt 	isect_t;	/* distance to intersection */
	int 	isect_enter;	/* entering? ie hit front? */
	Object 	*isect_prim;	/* object we hit */
	Surface *isect_surf;	/* surface def of hit object */
	Object	*isect_self;	/* pointer to self for queue elimination */
} Isect;

typedef struct t_object {
	unsigned short 	o_type;
	Flt             o_dmin[NSLABS];
	Flt             o_dmax[NSLABS];
	struct t_objectprocs {
		int 	(*intersect) (t_object*, Ray*, Isect*);
		void    (*normal) (t_object*, Isect*, Flt*, Flt*);
	} * o_procs;
	Surface         *o_surf;
	void            *o_data;
	Clip            *clips;
	struct t_object *next;          /* next object in original list, sibling */
} Object;

typedef struct t_compositedata {
	unsigned long   size;
	Object          *children;
} CompositeData;

typedef struct t_object::t_objectprocs ObjectProcs;

typedef struct t_pixel {
	unsigned char r, g, b, q;
} Pixel;

typedef struct t_transform {
	Matrix                  mat;
	struct t_transform      *next;
} Transform;

typedef unsigned char   Palette[256][3];

typedef struct t_background {
	Color   color;
	Vec     up;
	Palette pal;
} Background;

/* preprocessor macro structure */
typedef struct t_macro {
	char            *macro,         /* define'd word */
			*text;          /* text to replace macro with */
	int             mlen,           /* length of macro */
			tlen;           /* length of text */
	struct t_macro  *next;          /* stack link */
} Macro;

#ifndef PI
#define PI 		(3.14159265358979323844)
#endif /* PI */

#define degtorad(x)     (((Flt)(x))*PI/180.0)

/* primitive types */

#define		T_COMPOSITE	(0)
#define		T_SPHERE	(1)
#define		T_POLY		(2)
#define		T_CONE		(3)
#define		T_RING		(4)
#define		T_TRI		(5)
#define         T_FUZZY         (6)

/* camera projections types */

#define         P_FLAT          (0)
#define         P_ORTHOGRAPHIC  (1)
#define         P_FISHEYE       (2)
#define         P_NO_PARALLAX   (3)

/* antialiasing mode */

#define         A_NONE          (0)
#define         A_CORNERS       (1)
#define         A_QUICK         (2)
#define         A_ADAPTIVE      (3)
