/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                      PlotEqn3.B = PlotEqn3 Database                     บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                           ALL RIGHTS RESERVED                           บ
บ                                                                         บ
บ                Requires : color.bc, greys.map, ploteqn3.bo              บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio {
	from -30 -30 30
	at 16 16 0
	up 0 0 1
	angle 70
	res 640 480
	antialias adaptive
	aspect 1.333
	ambient .1 .1 .1

        depth 1
}

light {
	type point
	falloff 0
	position 0 -12 6
	color .5 .5 .5
}

light {
	type point
	falloff 0
	position -8 -8 8
	color .5 .5 .5
}

//      Equation

surface {
	diffuse yellow
	spec .5 .5 .5
	shine 240 white
}

#include ploteqn3.bo
