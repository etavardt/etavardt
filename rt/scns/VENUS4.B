/*
�������������������������������������������������������������������������ͻ
�                                                                         �
�             Venus.B = Venus Sculpture (model by Mira Imaging)           �
�                                                                         �
�          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         �
�                           ALL RIGHTS RESERVED                           �
�                                                                         �
�                Requires : color.bc, greys.map, venus.bo                 �
�                                                                         �
�������������������������������������������������������������������������ͼ
*/

#include color.bc

studio {
	from 2 -20 4
	at 0 0 0
	up 0 0 1
	angle 60
	res 640 480
	antialias adaptive
	threshold 2
	aspect 1.333
	background {
		greys.map
		up 0 0 -1
	}
	ambient .1 .1 .1
}

//      light sources

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

//      water surface

surf {
	diff 0 .1 .05
	spec .05 .15 .1
	transparent .5 .8 .7
	ior 1.2
	shine 100 white
	bump {
		wave { center -.5 .2 -.8 wavelength 1.3 amp .2 }
		wave { center .6 -.3 -.8 wavelength .9 amp .1 }
	}
}

ring { center 0 0 -1 normal 0 0 1 radius 10000 }

//      metallic venus surface

surface {
	bump {
		turbulence {
			scale .015 .0015 .0015
			amp .5
			terms 10
		}
	}
	diffuse steel_blue
	spec .4 .4 .4
	shine 10 gold
}

#include venus.bo
