/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                    Hex2.B = Hex Patterned Spheres                       บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                          ALL RIGHTS RESERVED                            บ
บ                                                                         บ
บ                   Requires : color.bc, hexgrid2.bo                      บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio  {
	from 20 -20 10
	at  0 0 0
	up  0 0 1
	angle  60
	res  640 480
	antialias adaptive
	threshold 2
	aspect  1.3333
	background {
		greys.map
	}
}

light {
	type point
	falloff 1
	position 20 5 40
	color 40 40 40
}
light {
	type point
	falloff 1
	position -10 -20 40
	color 40 40 40
}


surf { diff .3 .3 .3 spec light_steel_blue shine 1000 white }
sphere { center 1 8 2 radius 6 }

surf { diff .3 .3 .3 spec gold shine 1000 white }
sphere { center -4 -2 7 radius 4 }

#include hexgrid2.bo
