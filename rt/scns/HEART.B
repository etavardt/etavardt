/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                 Heart.B = Two views of the Human Heart                  บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                          ALL RIGHTS RESERVED                            บ
บ                                                                         บ
บ                    Requires : color.bc, heart.bo                        บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio {
	from -3 -8 14
	at -3 0 4
	up 0 0 1
	angle 44
	res 640 480
	antialias adaptive
	threshold 2
	aspect 1.333
	ambient .15 .15 .15
}

light {
	type point
	falloff 0
	position 2 15 15
	color .5 .5 .5
}

light {
	type point
	falloff 0
	position -2 -4 5
	color .5 .5 .5
}

light {
	type point
	falloff 0
	position -10 -5 6
	color .6 .6 .6
}

light {
	type point
	falloff 0
	position 10 -5 6
	color .6 .6 .6
}

//      heart

surface {
	diffuse brilliant_red
	spec .3 .3 .3
	shine 300 white
}

#include heart.bo


//      rotated heart

surface {
	diffuse brilliant_red
	spec .3 .3 .3
	shine 300 white
}

transform {
	rotate 0 0 -90
	translate -9 1 0
}
#include heart.bo
transform_pop
