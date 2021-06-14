/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ         Faces.B = Mix of Faces and Skull (models of a man's face,       บ
บ		Nefertiti's face and a human skull by Mira Imaging)       บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                         ALL RIGHTS RESERVED                             บ
บ                                                                         บ
บ    Requires : color.bc, greys.map, mansface.bo, nefertt.bo, skull2.bo   บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio {
	from 13 0 0
	at 0 0 -1
	up 0 0 1
	angle 60
	res 640 480
	antialias adaptive
	threshold 2
	aspect 1.0
	background {
		sky.map
		up 0 0 -1
	}
	ambient .1 .1 .1
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

light {
	type point
	falloff 0
	position 20 0 0
	color .5 .5 .5
}



//	ground plane

surface {
	diff .44 .66 .78
	shine 100 white
	bump {
		wave {
			center 0 0 0
			wavelength 1
			amp .2
		}
		wave {
			center 1 2 .4
			wavelength .7
			amp .16
		}
		wave {
			center -4 7 1.4
			wavelength .4
			amp .12
		}
	}
}
ring {
	center 0 0 -7
	normal 0 0 1
	radius 20000
}





//      man's face

surface {
	diffuse white
	spec .3 .3 .3
	shine 50 white
}

#include mansface.bo


//      Nefertiti

surface {
	diffuse white
	spec .3 .3 .3
	shine 50 white
}

transform {
	rotate 0 0 110
	translate 0 -5.25 0
}
#include nefertt.bo
transform_pop


//      human skull

surface {
	diffuse white
	spec .3 .3 .3
	shine 50 white
}

transform {
	rotate 0 0 90
	translate 0 5.25 0
}
#include skull2.bo
transform_pop
