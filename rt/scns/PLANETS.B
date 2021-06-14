/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ  Planets.B = Earth, Moon, M31 (Andromeda Galaxy), M27 (Dumbbell Nebula) บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                          ALL RIGHTS RESERVED                            บ
บ                                                                         บ
บ   Requires : color.bc, greys.map, ccdmoon.img, ccdm31.img, ccdm27.img   บ
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


//      the sun

light {
	type point
	falloff 0
	position 30 0 0
	color 8 8 8
}


//      the Earth

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


//      the CCD moon image

surface {
	diff {
		image ccdmoon.img
		position -16 45 0
		across 1 0 0
		normal 0 -1 0
		scale 16
	}
}

polygon { points 4
	vertex -16 45 -16
	vertex -16 45   0
	vertex   0 45   0
        vertex   0 45 -16
}


//      the CCD M31 image - the Andromeda galaxy

surface {
	diff {
		image ccdm31.img
		position  6 45 8
		across 1 0 0
		normal 0 -1 0
		scale 16
	}
}

polygon { points 4
	vertex   6 45 -8
	vertex   6 45  8
	vertex  24 45  8
        vertex  24 45 -8
}


//      the CCD M27 image - the Dumbbell nebula

surface {
	diff {
		image ccdm27.img
		position 30 45 0
		across 1 0 0
		normal 0 -1 0
		scale 16
	}
}

polygon { points 4
	vertex  30 45 -16
	vertex  30 45   0
	vertex  46 45   0
        vertex  46 45 -16
}
