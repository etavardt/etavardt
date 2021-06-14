/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                    Tiles1.B = Tiles and Geometric forms                 บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                           ALL RIGHTS RESERVED                           บ
บ                                                                         บ
บ              Requires : color.bc, greys.map, cube.bo, tiles.bo          บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio {
	from  8 -5 8
	at 4.5 4.5 -2
	up 0 0 1
	angle 50
	res 640 480
	antialias adaptive
	threshold 2
	aspect 1.333
	background {
		greys.map
	}
	ambient .1 .1 .1
}

//      light sources

light {
	type point
	falloff 1
	position -3 4 10
	color 9 9 9
}

light {
	type point
	falloff 1
	position 12 -5 10
	color 9 9 9
}

//      base surface

surf { diff dark_plum }
ring { center 0 0 -0.1 normal 0 0 1 radius 1000 }

//      chrome objects

surf {
	diff light_steel_blue
	spec .5 .5 .5
	shine 100
}

sphere { center 7 5 1.1 radius 1 }

cone { base 3 4 .1 apex 3 4 1 base_radius .6 apex_radius 1 }
cone { base 3 4 1 apex 3 4 2.5 base_radius 1 apex_radius .5 }
ring { center 3 4 2.5 normal 0 0 1 radius .5 }

transform { scale .5 rotate 0 0 40 translate 6 2 .6 }
#include cube.bo
transform_pop

//      tile surface

surf {
	texture {
		pattern spherical
		turbulence {
			terms 6
			amp 6
		}
		blend .8
		radius 1
		radius 3
		surf { diff 0 .3 .6 shine 20 white }
		surf { diff 0 .1 .2 shine 20 white }
	}
}

#include tiles.bo
