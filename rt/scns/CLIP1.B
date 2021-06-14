/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                    Clip1.B = Object Clipping Test                       บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                          ALL RIGHTS RESERVED                            บ
บ                                                                         บ
บ                    Requires : color.bc, greys.map                       บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

//      clip1.b -- test out new clipping planes

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
ring { center 0 0 0 normal 0 0 1 radius 1000 }

//      clip spheres

surf {
	diff 0 .5 1
	shine 20 white
}

sphere {
	center -1.5 0 .5
	radius .5
	clip {
		center -1.5 0 .2
		normal 0 0 -1   // only keep bottom part
	}
}
sphere {
	center -0.5 0 .5
	radius .5
	clip {
		center -0.5 0 .4
		normal 0 0 -1   // only keep bottom part
	}
}
sphere {
	center 0.5 0 .5
	radius .5
	clip {
		center 0.5 0 .6
		normal 0 0 -1   // only keep bottom part
	}
}
sphere {
	center 1.5 0 .5
	radius .5
	clip {
		center 1.5 0 .8
		normal 0 0 -1   // only keep bottom part
	}
}


sphere {        //      keep bottom wedge
	center -1.5 -1 .5
	radius .5
	clip {
		center -1.5 -1 .2
		normal 1 0 -1   // only keep bottom part
	}
	clip {
		center -1.5 -1 .2
		normal -1 0 -1   // only keep bottom part
	}
}

//      use two hemispheres to make a cup with thickness

sphere {
	center 0 -1 .5
	radius .5
	clip {
		center 0 -1 .5
		normal 0 0 -1   // only keep bottom part
	}
}
sphere {
	center 0 -1 .5
	radius .4
	clip {
		center 0 -1 .5
		normal 0 0 -1   // only keep bottom part
	}
}
ring {          // edge of clip cup
	center 0 -1 .5
	normal 0 0 1
	min_radius .4   // radius of inner hemisphere
	max_radius .5   // radius of outer hemisphere
}

sphere {        // keep ring around center
	center 1.5 -1 .5
	radius .5
	clip {
		center 1.5 -1 .6
		normal 0 0 -1   // toss top
	}
	clip {
		center 1.5 -1 .4
		normal 0 0 1    // toss bottom
	}
}

