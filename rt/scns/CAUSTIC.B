/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ             Caustic.B = Trasparent Objects Faked Caustics Test          บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                          ALL RIGHTS RESERVED                            บ
บ                                                                         บ
บ                          Requires : color.bc                            บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio {
	from 8 2 7
	at 0 0 1.0
	up 0 0 1
	angle 30
	res 640 480
	antialias adaptive
	aspect 1.3333
	background black
	ambient .1 .1 .1

	caustics
}

light {
	type point
	falloff 1
	position 0 1 5
	color 5 5 5
}

surface {
	texture {
		pattern checker
		scale .8 .8 0
		surf { diff white }
		surf { diff medium_plum }
	}
}
ring {
	center 0 0 0
	normal 0 0 1
	radius 1000
}

surface {
	spec .2 .2 .2
	shine 100 white
	transparent .6 .6 .6
	ior 1.2
}
sphere { center 0 0 1.5 radius 1.0 }
sphere { center 0 1 2.5 radius .5 }
