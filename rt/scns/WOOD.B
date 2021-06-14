/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                Wood.B = Wood Textured Block with Clipping               บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                          ALL RIGHTS RESERVED                            บ
บ                                                                         บ
บ                     Requires : color.bc, block.bo                       บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio {
	from -20 -20 15
	at -2 0 5
	up 0 0 1
	angle 45
	res 640 480
	antialias adaptive
	threshold 4
	aspect 1.333
	background {
		greys.map
	}
	ambient .1 .1 .1
}

light {
	type point
	falloff 0
	position -15 -13 10
	color .8 .8 .8
}
light {
	type point
	falloff 0
	position -10 20 30
	color .6 .6 .6
}

//      wood surface

surface  {
	texture  {
		turbulence  {
			scale  .1 .9 1
			amp  .3
			terms  3
		}
		pattern  spherical
		scale  0 1 1
		translate  0 0 5
		blend  .5
		radius  .15
		radius  .06

		surf {
			texture {
				pattern noise
				scale 3 .03 .03
				terms 2
				surf { diff brown shine 100 white }
				surf { diff medium_tan shine 100 white }
			}
		}
		surf {
			texture {
				pattern noise
				scale 3 .03 .03
				terms 2
				surf { diff dim_brown shine 20 white }
				surf { diff dark_tan shine 20 white }
			}
		}
	}
}

#include block.bo
