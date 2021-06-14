/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ               Amp.B = How Amplitude Affects Turbulence Test             บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                          ALL RIGHTS RESERVED                            บ
บ                                                                         บ
บ                       Requires : color.bc, greys.map                    บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio {
	from 0 -20 0
	at 0 0 0
	up 0 0 1
	angle 25
	res 640 480
	antialias adaptive
	aspect 1.333
	background {
		greys.map
	}
	ambient .1 .1 .1
}

//      light sources

light {
	type point
	falloff 1
	position 5 -6 12
	color 6 6 6
}

light {
	type point
	falloff 1
	position -1 -6 2
	color 4 4 4
}

//      the textured spheres

#define SURF \
surf { \
	texture { \
		pattern spherical \
		turbulence { \
			terms 6 \
			amp AMP \
		} \
		blend .8 \
		radius .5 \
		radius 1 \
		surf { diff 0 .1 .2 } \
		surf { diff white } \
	} \
}

#define AMP 0
SURF
#undef AMP
sphere { center -2 0 2 radius 1.2 }
#define AMP .5
SURF
#undef AMP
sphere { center  0 0 2 radius 1.2 }
#define AMP 1
SURF
#undef AMP
sphere { center  2 0 2 radius 1.2 }
#define AMP 2
SURF
#undef AMP
sphere { center -2 0 0 radius 1.2 }
#define AMP 3
SURF
#undef AMP
sphere { center  0 0 0 radius 1.2 }
#define AMP 5
SURF
#undef AMP
sphere { center  2 0 0 radius 1.2 }
#define AMP 8
SURF
#undef AMP
sphere { center -2 0 -2 radius 1.2 }
#define AMP 12
SURF
#undef AMP
sphere { center  0 0 -2 radius 1.2 }
#define AMP 20
SURF
#undef AMP
sphere { center  2 0 -2 radius 1.2 }
