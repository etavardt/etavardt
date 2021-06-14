/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                         Col1.B = Test of Columns                        บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                           ALL RIGHTS RESERVED                           บ
บ                                                                         บ
บ                 Requires : color.bc, greys.map, column.bo               บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio {
	from -5 -10 7
	at 1.5 0 1.5
	up 0 0 1
	angle 50
	res 640 480
	antialias adaptive
	threshold 4
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
	position -5 -3 12
	color 8 8 8
}

light {
	type point
	falloff 1
	position 1 -6 12
	color 8 8 8
}

//      ground plane

surf {
	spec .5 .5 .5
	fuzz 0.05
	no_antialias  // speed things up a bit - since it is a fuzzy surface
}

ring { center 0 0 0 normal 0 0 1 radius 1000 }

//     two columns

surf {
	texture {
		pattern spherical
		turbulence {
			terms 7
			amp 6
		}
		blend .8
		radius .5
		radius 2
		surf { diff 0 .1 .2  shine 20 white }
		surf { diff .8 .8 .8 shine 20 white }
	}
}

transform { scale 0.1 }

transform { rotate 0 0 35 }
#include column.bo
transform_pop

transform { rotate 90 0 0 translate 27 20 12 }
#include column.bo
transform_pop

transform_pop

