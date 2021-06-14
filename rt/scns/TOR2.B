/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                      Tor2.B = Display a Torus                           บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                          ALL RIGHTS RESERVED                            บ
บ                                                                         บ
บ                    Requires : color.bc, t16_16p.bo                      บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

	Requires t16_16p.bo to render.  Create using
	  the command:  torus 3 2 16 16 patches >t16_16p.bo

*/

#include color.bc

studio  {
	from 17 -20 8
	at  0 0 0
	up  0 0 1
	angle  40
	res 640 480
	antialias adaptive
	projection flat
	aspect  1.3333
	background {
		greys.map
	}
	no_shadows
}

light  {
	type  point
	falloff  0
	position 2 -6 5
	color  .6 .6 .6
}

light  {
	type  point
	falloff  0
	position -4 -14 7
	color  .6 .6 .6
}

surface {
	diff 0 .5 1
	shine 10 white
}

transform { scale 1 1 .2
	    translate 6 0 0
}
#include t16_16p.bo
transform_pop

transform { scale 1 .7 2.5
	    translate -6 0 0
	    rotate 90 0 0
}
#include t16_16p.bo
transform_pop
