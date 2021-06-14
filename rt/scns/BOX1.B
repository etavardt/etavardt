/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                           Box1.B = Puzzle Box                           บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                           ALL RIGHTS RESERVED                           บ
บ                                                                         บ
บ                  Requires : color.bc, greys.map, box.bo                 บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio {
	from -25 -15 10
	at 0 0 -3
	up 0 0 .7
	angle 50
	res 640 480
	antialias adaptive
	threshold 2
	aspect 1.333

	background black
}

//      light sources

light {
	type point
	falloff 0
	position -20 20 30
	color .9 .9 .9
}

light {
	type point
	falloff 0
	position -30 -15 30
	color .9 .9 .9
}

light {
	type point
	falloff 0
	position 50 50 50
	color .9 .9 .9
}

//      base surface

surf {
	spec .4 .4 .4
	shine 10 gold
	bump {
		turbulence {
			scale 2 2 2
			amp .3
			terms  4
		}
	}
}
ring { center 0 0 -7 normal 0 0 1 radius 10000 }

//      wood sphere inside box

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
sphere { center 0 0 0 radius 5.8 }

//      the box

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
		surf { diff 0 .1 .2 shine 20 white }
		surf { diff .8 .8 .8 shine 20 white }
	}
}

#include box.bo

transform { rotate 0 0 180 }
#include box.bo
transform_pop

transform { rotate 0 0 90 }
#include box.bo
transform_pop

transform { rotate 0 0 -90 }
#include box.bo
transform_pop

transform { rotate 0 90 0 }
#include box.bo
transform_pop

transform { rotate 0 -90 0 }
#include box.bo
transform_pop
