/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                    Basin1.B = Drown the Utah Teapot                     บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                          ALL RIGHTS RESERVED                            บ
บ                                                                         บ
บ    Requires : color.bc, teapot.bo, basin1.bo, sphflk3.bo, column.bo     บ
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

//      marble column

surf {                  // marble
	texture {
		pattern spherical
		translate 0 1 0
		turbulence {
			terms 7
			amp 6
		}
		blend .8
		radius .5
		radius 2
		surf { diff 0 .1 .2 }
		surf { diff .8 .8 .8 }
	}
}

transform { scale .05 rotate 0 0 30 translate 0 1 0 }
#include column.bo
transform_pop

//      sphereflake

surf {
	diff steel_blue
	spec steel_blue
	shine 50 white
}

transform { translate 0 -1 .5 }
#include sphflk3.bo
transform_pop

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

//      teapot

surf {
	diff 0 .5 1
	shine 20 white
}

transform { rotate 10 5 -20 scale 0.2 translate -2.2 .2 .85 }
#include teapot.bo
transform_pop
