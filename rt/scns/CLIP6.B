/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                    Clip6.B = Object Clipping Test                       บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                          ALL RIGHTS RESERVED                            บ
บ                                                                         บ
บ                    Requires : color.bc, greys.map                       บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

//      clip6.b -- test out clipping cones

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
	diff dark_plum           // dark plum
}
ring { center 0 0 0 normal 0 0 1 radius 1000
	clip { apex -2 0 0 base 2 0 0 radius .3 outside }
	clip { apex -2 1 .2 base 2 1 -.2 radius .3 outside }
	clip { apex -2 -1 -.5 base 2 -1 .5 radius .3 outside }
}

transform { rotate 0 30 0 }

surf { diff 0 .5 1 }
ring { center 0 0 0 normal 0 0 1 radius 1000
	clip { apex -2 0 0 base 2 0 0 radius .3 inside }
}
ring { center 0 0 0 normal 0 0 1 radius 1000
	clip { apex -2 1 .2 base 2 1 -.2 radius .3 inside }
}
ring { center 0 0 0 normal 0 0 1 radius 1000
	clip { apex -2 -1 -.5 base 2 -1 .5 radius .3 inside }
}


