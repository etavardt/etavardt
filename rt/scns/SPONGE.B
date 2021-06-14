/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                       Sponge.B = Menger Sponge                          บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                          ALL RIGHTS RESERVED                            บ
บ                                                                         บ
บ                    Requires : color.bc, sponge.bo                       บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio {
	from 10 4 5
	at 0 0 -0.5
	up 0 0 1

	angle 50
	res 640 480
	antialias adaptive
	aspect 1.3333

	background black
	ambient .2 .2 .2

        depth 1
}

//      light sources

light {
	type point
	falloff 1
	position 10 6 10
	color 1 1 1
}


//      ground level

surface { diff light_gray }

ring {
	position 0 0 -4
	normal 0 0 1
	radius 20000
}


//      Menger Sponge

surf {
        amb .1 .1 .1
	diff 0 .5 1
	shine 20 white
}

#include sponge.bo
