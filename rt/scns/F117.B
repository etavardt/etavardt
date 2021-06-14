/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ               F117.B = F117 Aircraft (model by Mira Imaging)            บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                           ALL RIGHTS RESERVED                           บ
บ                                                                         บ
บ                   Requires : color.bc, greys.map, f117.bo               บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio {
	from -3 -13 6
	at 0 0 5
	up 0 0 1
	angle 60
	res 640 480
	antialias adaptive
	threshold 4
	haze 0.03
	aspect 1.333
	ambient .1 .1 .1
}

light {
	type point
	falloff 0
	position -3 -25 9
	color .5 .5 .5
}

light {
	type point
	falloff 0
	position -8 -8 8
	color .5 .5 .5
}


//      the sky

surf {
	texture {
		pattern checker
		scale 14 23 0
		blend .8
		turbulence {
			amp 3
			terms 4
		}
		surface { amb white }
		surface { amb sky_blue }
     }
}

ring {
	center 0 0 10
	normal 0 0 -1
	radius 20000
}


//      f117

surface {
	diffuse dim_grey
	spec .3 .3 .3
	shine 240 white
}

transform {
	rotate 30 20 20
	translate 0 0 4
}
#include f117.bo
transform_pop
