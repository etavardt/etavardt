/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                    Focus.B = Depth of Field / Focus                     บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                             ALL RIGHTS RESERVED                         บ
บ                                                                         บ
บ                          Requires : color.bc                            บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio {
	from 8 0 3
	at 0 0 0
	up 0 0 1
	angle 45
	res 640 480
	antialias adaptive
	threshold 2
	aspect 1.3333

	aperture .2
	focal_length 8.5
	samples 4

	background black
	ambient .1 .1 .1
}

//      light sources

light {
	type point
	falloff 0
	position -2 1 4
	color .7 .7 .7
}

light {
	type point
	falloff 0
	position 10 1 8
	color .4 .4 .4
}

//      ground plane

surface {
	texture {
		pattern checker
		scale 1 1 0
		surf { diff white }
		surf { diff red }
	}
}

ring {
	center 0 0 0
	normal 0 0 1
	radius 1000
}

//      sample spheres to show focus

surface { spec white shine 30 }
sphere { center 0 0 0.5 radius 1.0 }
sphere { center -3 1 1.5 radius 1.0 }
sphere { center 3 -1 1.0 radius 1.0 }
