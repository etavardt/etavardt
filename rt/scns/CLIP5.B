/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                    Clip5.B = Object Clipping Test                       บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                          ALL RIGHTS RESERVED                            บ
บ                                                                         บ
บ                    Requires : color.bc, greys.map                       บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

//      clip5.b -- test out clipping spheres

#include color.bc

studio {
	from 1 -6 4
	at 0 -0.5 0.5
	up 0 0 1
	angle 35
	res 640 480
	antialias adaptive
	aspect 1.333
	background {
		greys.map
	}
	ambient .2 .2 .2
}

light {
	type point
	falloff 0
	position 1 -.5 5
	color .5 .5 .5
}
light {
	type point
	falloff 0
	position -2 -4 5
	color .5 .5 .5
}

//      base surface

surface {
	diff dark_plum           // dark plum
}
ring { center 0 0 0 normal 0 0 1 radius 1000 }

//      clip spheres

surf {
	diff 0 .5 1
	shine 20 white
}

sphere {
	center -.5 0 .5
	radius .5
	clip {
		center -.5 0 1
		radius .2
		inside
	}
}
sphere {
	center 0.5 0 .5
	radius .5
	clip {
		center .5 0 1
		radius .2
		outside
	}
	clip { center 0 0 .5 normal 0 0 1 }
}
cone {
	apex -1 -1 .5
	base 1 -1 .5
	radius .3
	clip { center 0 -1 .8 radius .5 outside }
	clip { center 0 -1 0 radius 1 inside }
}
