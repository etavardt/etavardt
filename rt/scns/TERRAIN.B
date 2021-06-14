/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ             Terrain.B = Fractal Terrain via Plasma Generation           บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                           ALL RIGHTS RESERVED                           บ
บ                                                                         บ
บ                 Requires : color.bc, greys.map, terrain.bo              บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio {
	from -15 -15 40
	at 30 30 12
	up 0 0 1
	angle 75
	res 640 480
	antialias adaptive
	aspect 1.333
	ambient .05 .05 .05

        depth 1
}

light {
	type point
	falloff 0
	position -15 -15 40
	color .4 .4 .4
}

light {
	type point
	falloff 0
	position  40 -10 8
	color .5 .5 .5
}

//

surface {
	diffuse slate_blue
	spec .7 .7 .7
	shine 300 white
}

#include terrain.bo
