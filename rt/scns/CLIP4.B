/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                    Clip4.B = Object Clipping Test                       บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                          ALL RIGHTS RESERVED                            บ
บ                                                                         บ
บ                Requires : color.bc, greys.map, clips.bo                 บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

//      clip4.b -- test out new clipping planes with transforms

#include color.bc

studio {
	from 1 -6 4
	at 0 0 0
	up 0 0 1
	angle 35
	res 640 480
	antialias adaptive
	aspect 1.333
	background {
		greys.map
	}
	ambient .2 .2 .2
}

light {
	type point
	falloff 0
	position 1 -.5 5
	color .5 .5 .5
}
light {
	type point
	falloff 0
	position -2 -4 5
	color .5 .5 .5
}

//      base surface

surface {
	diff dark_plum          // dark plum
}
ring { center 0 0 0 normal 0 0 1 radius 1000 }

//      clip spheres

surf {
	diff 0 .5 1
	shine 20 white
}

transform { rotate 0 0 0 translate -2 0 .5 }
#include clips.bo
transform_pop

transform { rotate 72 0 0 translate -1 0 .5 }
#include clips.bo
transform_pop

transform { rotate 144 0 0 translate 0 0 .5 }
#include clips.bo
transform_pop

transform { rotate 216 0 0 translate 1 0 .5 }
#include clips.bo
transform_pop

transform { rotate 288 0 0 translate 2 0 .5 }
#include clips.bo
transform_pop
