/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                    Light.B = Soft Shadow Lighting Test                  บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                           ALL RIGHTS RESERVED                           บ
บ                                                                         บ
บ                      Requires : color.bc, greys.map                     บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio {
	from 5 -20 20
	at 0 0 0
	up 0 0 1
	angle 50
	res 640 480
	antialias adaptive
	threshold 2
	aspect 1.333
	background {
		greys.map
	}
	ambient black
}

surf { diff white }

//      white floor

ring { center 0 0 0 normal 0 0 1 radius 10000 }

//      light body

surf {
	ambient white
	transparent white
}

cone { apex -6 0 10 base 6 0 10 radius 1 }

//      lights inside tube

light { type spherical radius .5 samples 2 falloff 2 color 10 10 10 position -5 0 10 }
light { type spherical radius .5 samples 2 falloff 2 color 10 10 10 position -4 0 10 }
light { type spherical radius .5 samples 2 falloff 2 color 10 10 10 position -3 0 10 }
light { type spherical radius .5 samples 2 falloff 2 color 10 10 10 position -2 0 10 }
light { type spherical radius .5 samples 2 falloff 2 color 10 10 10 position -1 0 10 }
light { type spherical radius .5 samples 2 falloff 2 color 10 10 10 position  0 0 10 }
light { type spherical radius .5 samples 2 falloff 2 color 10 10 10 position  1 0 10 }
light { type spherical radius .5 samples 2 falloff 2 color 10 10 10 position  2 0 10 }
light { type spherical radius .5 samples 2 falloff 2 color 10 10 10 position  3 0 10 }
light { type spherical radius .5 samples 2 falloff 2 color 10 10 10 position  4 0 10 }
light { type spherical radius .5 samples 2 falloff 2 color 10 10 10 position  5 0 10 }

//      objects

surf {
	diff .5 .5 .5
	spec .5 .5 .5
}

sphere { center 4 2 1 radius 1 }
sphere { center -4 -2 2 radius 2 }
sphere { center 1 -5 3 radius 3 }
cone { apex -2 3 0 base -2 3 4 radius 1 }
ring { center -2 3 4 normal 0 0 1 radius 1 }
cone { base 6 3 0 apex 6 3 4 base_radius 1 apex_radius 0 }
