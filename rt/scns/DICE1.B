/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                 Dice1.B = Dice with Clipped-Sphere Dimples              บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                            ALL RIGHTS RESERVED                          บ
บ                                                                         บ
บ                    Requires : color.bc, sky.map, die2.bo                บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio  {
	from 6 -10 7
	at  -1 0 3.2
	up  0 0 1
	angle  55
	res  640 480
	antialias adaptive
	projection flat
	aspect  1.3333
	background  { sky.map }
	ambient  .2 .2 .2
}

//      light sources

light { type spherical radius .5 samples 2 falloff 2 color 5 5 5 position -2 -3 7 }

light { type spherical radius .5 samples 2 falloff 2 color 8 8 8 position 8 4 7 }


//      ground plane

surface  {
	texture  {
		pattern spherical
		radius 1
		radius .3
		blend  .5
		surface  { diff  white shine  20 }
		surface  { diff  dark_slate_gray shine  20 }
		turbulence  {
			terms  6
			amp  3
		}
	}
}

ring  {
	center  0 0 0
	normal  0 0 1
	radius  10000
}

//      chrome sphere

surf { diff gold spec .5 .5 .5 shine 100 white }
sphere { center -6 4 4 radius 4 }

//      bring on the dice

#define DIE_BODY_SURF   surf { diff .7 .7 .7 shine 10 white }
#define DIE_SPOT_SURF   surf { diff red shine 10 white }

transform {
	rotate 45 45 45
	translate 0 0 1.732       //      put it up on a point
}
#include die2.bo
transform_pop

transform {
	rotate 180 90 30
	translate 2 4 1
}
#include die2.bo
transform_pop

transform {
	rotate 0 90 5
	translate 2 4 3
}
#include die2.bo
transform_pop

transform {
	rotate 0 0 -20
	translate -4 -2 1
}
#include die2.bo
transform_pop
