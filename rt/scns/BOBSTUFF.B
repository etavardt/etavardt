/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                      BobStuff.B = Bob Logo and Stuff                    บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                            ALL RIGHTS RESERVED                          บ
บ                                                                         บ
บ                 Requires : color.bc, greys.map, boblogo.bo              บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio {
	from 100 -100 35
	at 20 0 8
	up 0 0 1
	angle 16
	res 640 480
	aspect 1.333
	antialias adaptive
	threshold 2
	jitter
	background sky_blue
}

light {
	type point
	falloff 0
	position 0 -20 40
	color .6 .6 .6
}
light {
	type point
	falloff 0
	position 35 -10 30
	color .6 .6 .6
}
light {
	type point
	falloff 0
	position 20 10 50
	color .6 .6 .6
}
light {
	type point
	falloff 0
	position -100 -100 10
	color .3 .3 .3
}

//      base plane

surf {
	texture {
		pattern checker
		scale 15 15 0
		surf { diff white }
		surf { diff black }
	}
}
ring {
	center 0 0 -17
	normal 0 0 1
	radius 5000
}

//      chrome ball

surface {
	diff steel_blue
	spec .4 .4 .5
	shine 30
}
sphere { center 10 20 10 radius 9 }

//      cluster of reflecting spheres

surf { diff .5 .3 .1 spec .5 .5 .5 shine 400 }
sphere { center 20 16 10 radius 4 }
sphere { center 28 20 12 radius 4 }
sphere { center 32 14  8 radius 4 }
sphere { center 30 10  4 radius 4 }

//      glass tubes

surface {
	transparent .9 .9 .9
	ior 1.3
	shine 30 .7 .7 .7
}
cone { apex 25 -1000 -15 apex_radius 1.0 base 25 1000 -15 base_radius 1.0 }
cone { apex 25 -1000 -9 apex_radius 1.0 base 25 1000 -9 base_radius 1.0 }
cone { apex 25 -1000 -3 apex_radius 1.0 base 25 1000 -3 base_radius 1.0 }
cone { apex 25 -1000 3 apex_radius 1.0 base 25 1000 3 base_radius 1.0 }
cone { apex 25 -1000 9 apex_radius 1.0 base 25 1000 9 base_radius 1.0 }
cone { apex 25 -1000 15 apex_radius 1.0 base 25 1000 15 base_radius 1.0 }
cone { apex 25 -1000 21 apex_radius 1.0 base 25 1000 21 base_radius 1.0 }

//      surface for Bob logo

surface {
	diff red
}

transform {
	scale 1.667 1.667 1.667
	rotate 90 0 0
	translate 0.0 0.0 0.0
}
#include boblogo.bo
transform_pop

