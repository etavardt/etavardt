/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                    Clip3.B = Object Clipping Test                       บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                          ALL RIGHTS RESERVED                            บ
บ                                                                         บ
บ                    Requires : color.bc, greys.map                       บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

//      clip3.b -- test out new clipping planes with rings, polygons

#include color.bc

studio {
	from 1 -6 5
	at 0 0 .5
	up 0 0 1
	angle 30
	res 640 480
	antialias quick
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

//      clip rings

surf {
	diff 0 .5 1
	shine 20 white
}

//      5 rings with same cliping plane

ring { center 0 0 0.1 normal 0 0 1 min_radius .3 max_radius 1 clip { center 0 0 .5 normal -1 1 -2 } }
ring { center 0 0 0.3 normal 0 0 1 min_radius .3 max_radius 1 clip { center 0 0 .5 normal -1 1 -2 } }
ring { center 0 0 0.5 normal 0 0 1 min_radius .3 max_radius 1 clip { center 0 0 .5 normal -1 1 -2 } }
ring { center 0 0 0.7 normal 0 0 1 min_radius .3 max_radius 1 clip { center 0 0 .5 normal -1 1 -2 } }
ring { center 0 0 0.9 normal 0 0 1 min_radius .3 max_radius 1 clip { center 0 0 .5 normal -1 1 -2 } }
