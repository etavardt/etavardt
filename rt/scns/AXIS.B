/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                   Axis.B = Coordinate Axes with Labels                  บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                          ALL RIGHTS RESERVED                            บ
บ                                                                         บ
บ                          Requires : color.bc                            บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio {
	from 60 -100 55
	at 15 0 15
	up 0 0 1
	angle 35
	res 640 480
	aspect 1.3333
	antialias adaptive
	threshold 4
	background black
	ambient .1 .1 .1
}

//      light sources

light { type point falloff 1 position -100 -100 100 color 25 25 25 }
light { type point falloff 1 position  100 -100 100 color 25 25 25 }
light { type point falloff 1 position -100  100 100 color 25 25 25 }
light { type point falloff 1 position  100  100 100 color 25 25 25 }

//      ground level

surface { diff plum }

ring {
	position 0 0 -6
	normal 0 0 1
	radius 5000
}

//      x-axis

surface { diff red shine 20 .5 .5 .5 }
//      arrow
cone { apex 0 0 0 base 25 0 0 apex_radius 1 base_radius 1 }
cone { apex 25 0 0 base 27 0 0 apex_radius 1 base_radius 5 }
cone { apex 27 0 0 base 35 0 0 apex_radius 5 base_radius 0 }
//      X
sphere { center 35 5 0 radius 1 }
sphere { center 42 5 0 radius 1 }
sphere { center 35 5 10 radius 1 }
sphere { center 42 5 10 radius 1 }
cone { apex 35 5 0 apex_radius 1
	 base 42 5 10 base_radius 1 }
cone { apex 35 5 10 apex_radius 1
	 base 42 5 0 base_radius 1 }

//      y-axis

surface { diff green shine 20 .5 .5 .5 }
//      arrow
cone { apex 0 0 0 base 0 25 0 apex_radius 1 base_radius 1 }
cone { apex 0 25 0 base 0 27 0 apex_radius 1 base_radius 5 }
cone { apex 0 27 0 base 0 35 0 apex_radius 5 base_radius 0 }
//      Y
sphere { center 5 35 10 radius 1 }
sphere { center 12 35 10 radius 1 }
sphere { center 8.5 35 0 radius 1 }
cone { apex 8.5 35 5 base 5 35 10 apex_radius 1 base_radius 1 }
cone { apex 8.5 35 5 base 12 35 10 apex_radius 1 base_radius 1 }
cone { apex 8.5 35 5 base 8.5 35 0 apex_radius 1 base_radius 1 }

//      z-axis

surface { diff blue shine 20 .5 .5 .5 }
//      arrow
cone { apex 0 0 0 base 0 0 25 apex_radius 1 base_radius 1 }
cone { apex 0 0 25 base 0 0 27 apex_radius 1 base_radius 5 }
cone { apex 0 0 27 base 0 0 35 apex_radius 5 base_radius 0 }
//      Z
sphere { center -5 -5 25 radius 1 }
sphere { center -12 -5 25 radius 1 }
sphere { center -5 -5 35 radius 1 }
sphere { center -12 -5 35 radius 1 }
cone { apex -5 -5 35 base -12 -5 25 apex_radius 1 base_radius 1 }
cone { apex -5 -5 35 base -12 -5 35 apex_radius 1 base_radius 1 }
cone { apex -5 -5 25 base -12 -5 25 apex_radius 1 base_radius 1 }

//      reflective sphere at 35,35,35

surf {
	diff steel_blue
	spec .5 .5 .5
	shine 30
}
sphere { center 35 35 35 radius 20 }


