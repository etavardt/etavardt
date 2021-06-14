/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                      Stack.B = Stack of Spheres                         บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                           ALL RIGHTS RESERVED                           บ
บ                                                                         บ
บ                    Requires : color.bc, greys.map                       บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio {
	from 6 12 6
	at 0 0 1
	up 0 0 1
	angle 25
	res 640 480
	aspect 1.333
	antialias adaptive
	background black
	ambient black
}

//      light sources

light {
	type point
	falloff 1
	position 4 6 5
	color  3 3 3
}

light {
	type point
	falloff 1
	position -7 3 5
	color 3 3 3
}

light {
	type point
	falloff 1
	position 3 -8 5
	color  3 3 3
}

//      ground plane

surface {
	texture {
		pattern checker
		scale 1 1 0
		surface { diff white spec .2 .2 .2 }
		surface { diff black spec .2 .2 .2 }
	}
}

ring {
	position 0 0 0
	normal 0 0 1
	radius 100
}

//      stack of spheres

surface { diff red spec .3 .3 .3 shine 500 }
sphere { center 0 1 .85 radius .85 }

surface { diff blue spec .3 .3 .3 shine 500}
sphere { center .85 -.5 .85 radius .85 }

surface { diff green spec .3 .3 .3 shine 500}
sphere { center -.85 -.5 .85 radius .85 }

surface { diff .2 .2 .2 spec .8 .8 .8 shine 500}
sphere { center 0 0 2.234 radius .85 }
