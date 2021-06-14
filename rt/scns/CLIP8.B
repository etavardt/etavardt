/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                    Clip8.B = Object Clipping Test                       บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                          ALL RIGHTS RESERVED                            บ
บ                                                                         บ
บ                    Requires : color.bc, greys.map                       บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

//      clip8.b -- test out clipping cones

#include color.bc

studio {
	from 3 3 3
	at 0 0 1
	up 0 0 1
	angle 60
	res 160 100
	antialias quick
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

//      outside surface

surf {
	diff 0 .5 1
	shine 20 white
}

sphere { center -1 -1 1 radius 1
	clip { apex -2 -2 1.5 base 1 3 1.5 radius .5 outside }
}
cone { apex 2 1.5 1 base -2 1.5 1 radius .5
	clip { apex -2 -2 1.5 base 1 3 1.5 radius .5 outside }
}
ring { center 2 1.5 1 normal -1 0 0 radius .5 }
ring { center -2 1.5 1 normal 1 0 0 radius .5 }


//      inside surface

surf {
	diff red
	shine 20 white
}

cone { apex -2 -2 1.5 base 1 3 1.5 radius .5
	clip { center -1 -1 1 radius 1 inside }
}
cone { apex -2 -2 1.5 base 1 3 1.5 radius .5
	clip { apex 2 1.5 1 base -2 1.5 1 radius .5 inside }
}




