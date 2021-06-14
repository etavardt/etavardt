/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                     Ducks.B = Ducks and a Basin                         บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                          ALL RIGHTS RESERVED                            บ
บ                                                                         บ
บ                Requires : color.bc, basin1.bo, duck.bo                  บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio {
	from 2 -8 6
	at -1 0 0.5
	up 0 0 1
	angle 35
	res 640 480
	antialias adaptive
	threshold 2
	aspect 1.333
	background {
		greys.map
	}
	ambient .1 .1 .1
}

light {
	type point
	falloff 0
	position -4 2 5
	color .4 .4 .4
}

light {
	type point
	falloff 0
	position -3 -4 4
	color .6 .6 .6
}

light {
	type point
	falloff 0
	position 3 -2 3
	color .4 .4 .4
}

//      base plane

surf {
	diff .5 .5 .5
	spec .5 .5 .5
	shine 100 white
	fuzz 0.02
}
ring { center 0 0 0 normal 0 0 1 radius 100 }

//      basin

surf {
	texture {
		pattern noise
		scale .04 .04 .04
		terms 4
		surf { diff white }
		surf { diff .3 .3 .3 }
	}
}

transform { translate -2 -.5 0 }
#include basin.bo

//      water surface

surf {
	diff .1 .2 .2
	transparent .7 .8 .9
	ior 1.2
	shine 100 white
	bump {
		//      primary wave
		wave { center -2.2 .2 1 wavelength .2 amp .2 damping .9 }
		//      reflected waves
		wave { center -2.85 .2 1 wavelength .2 amp .2 damping .9 }
		wave { center -1.15 .2 1 wavelength .2 amp .2 damping .9 }
		wave { center -2.2 .85 1 wavelength .2 amp .2 damping .9 }
		wave { center -2.2 -1.35 1 wavelength .2 amp .2 damping .9 }
	}
}
polygon { points 4
	vertex -.95 -1.45 1.1
	vertex -.95  1.45 1.1
	vertex  .95  1.45 1.1
	vertex  .95 -1.45 1.1
}

transform_pop


//      duckies

surf {
	diff yellow
	shine 20 white
}

// in the basin

transform { rotate 10 5 160 scale 0.2 translate -2.2 0 1.3 }
#include duck.bo
transform_pop

// on the ground

transform { rotate 10 5 20 scale 0.2 translate .6 .2 .85 }
#include duck.bo
transform_pop
