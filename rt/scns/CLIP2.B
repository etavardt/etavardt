/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                    Clip2.B = Object Clipping Test                       บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                          ALL RIGHTS RESERVED                            บ
บ                                                                         บ
บ                    Requires : color.bc, greys.map                       บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

//      clip2.b -- test out new clipping planes with cones

#include color.bc

studio {
	from 1 -6 5
	at 0 -1 0
	up 0 0 1
	angle 45
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
	diff dark_plum           // dark plum
}
ring { center 0 0 0 normal 0 0 1 radius 1000 }

//      clip cone

surf {
	diff 0 .5 1
	shine 20 white
}

cone {          //      45๘ diagonal clips at end of cylinder
	apex -2 0 .5
	base 2 0 .5
	radius .5
	clip {
		center -1.5 0 .5
		normal  1 1 0           // face normal inward toward the
	}                               // part we want to keep
	clip {
		center 1.5 0 .5
		normal -1 1 0
	}
}

cone {          //      clip in half, lengthwise
	apex -2 -1.1 .5
	base 2 -1.1 .5
	radius .5
	clip {
		center 0 -1.1 .5
		normal 0 0 -1   // keep the bottom half
	}
}

cone {          //      clip in half at a diagonal, lengthwise
	apex -2 -2.2 .5
	base 2 -2.2 .5
	radius .5
	clip {
		center 0 -2.2 .5
		normal -.3 0 -1   // keep the bottom half
	}
}

