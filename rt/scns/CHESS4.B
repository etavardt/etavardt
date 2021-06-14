/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ          Chess4.B = Marbled Chess Board w/ Pieces and Wine Glass        บ
บ             (chess piece models Copyright Randy Brown of UNC)           บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                          ALL RIGHTS RESERVED                            บ
บ                                                                         บ
บ       Requires : color.bc, greys.map, rook.bo, knight.bo, bishop.bo,    บ
บ                         queen.bo, king.bo, pawn.bo                      บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio  {
	from  10 -20 12
	at  0 -2 -1
	up  0 0 1
	angle  30
	res 640 480
	antialias adaptive
	threshold 2
	aspect  1.333
	background  black
	ambient .1 .1 .1
}

//      light sources

light  {
	type  point
	falloff  0
	position  20  -10 40
	color  .8 .8 .8
}

light  {
	type  point
	falloff  0
	position  -20 -30 10
	color  .9 .9 .9
}


//      table top

surface {
	diff steel_blue
	spec steel_blue
	shine 20 white
	bump {
		turbulence {
			scale .1 .1 .1
			amp .25
			terms 2
		}
	}
}

ring  { radius 50 center  0 0 -1 normal  0 0 1 }


//      define chess board

//      playing surface

surface  {
	texture  {
		pattern checker
		scale 1 1 0
		surface  { diff light_grey  shine  20 }
		surface  { diff black  shine  20 }
	}
}

polygon  {
	points  4
	vertex   4  4 0
	vertex   4 -4 0
	vertex  -4 -4 0
	vertex  -4  4 0
}

//      edges

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

polygon  {
	points  4
	vertex   4 -4 0
	vertex   4  4 0
	vertex   5  5 0
	vertex   5 -5 0
}
polygon  {
	points  4
	vertex   4  4 0
	vertex  -4  4 0
	vertex  -5  5 0
	vertex   5  5 0
}
polygon  {
	points  4
	vertex  -4 -4 0
	vertex   4 -4 0
	vertex   5 -5 0
	vertex  -5 -5 0
}
polygon  {
	points  4
	vertex  -4  4 0
	vertex  -4 -4 0
	vertex  -5 -5 0
	vertex  -5  5 0
}

//      rounded edges and corners

sphere  { center   5  5 -0.5 radius  .5 }
sphere  { center  -5  5 -0.5 radius  .5 }
sphere  { center   5 -5 -0.5 radius  .5 }
sphere  { center  -5 -5 -0.5 radius  .5 }

cone  { apex   5  5 -0.5 apex_radius  .5 base   5 -5 -0.5 base_radius  .5 }
cone  { apex   5 -5 -0.5 apex_radius  .5 base  -5 -5 -0.5 base_radius  .5 }
cone  { apex  -5 -5 -0.5 apex_radius  .5 base  -5  5 -0.5 base_radius  .5 }
cone  { apex  -5  5 -0.5 apex_radius  .5 base   5  5 -0.5 base_radius  .5 }


//      glass

surface {
	transparent .7 .7 .7
	ior 1.2
	shine 10 white
}

ring { center 2 -7 1.2 normal 0 0 1 radius .4 }
cone { apex 2 -7 1.2 apex_radius .4
	 base 2 -7 1.3 base_radius .6 }
cone { apex 2 -7 1.3 apex_radius .6
	 base 2 -7 3 base_radius .9 }
ring { center 2 -7 3 normal 0 0 1 min_radius .9 max_radius 1 }
cone { base 2 -7 3 base_radius 1
	 apex 2 -7 1.25 apex_radius .75 }
cone { base 2 -7 1.25 base_radius .75
	 apex 2 -7 1 apex_radius .5 }
cone { base 2 -7 1 base_radius .5
	 apex 2 -7 .9 apex_radius .1 }

//      glass stem
cone { base 2 -7 .9 base_radius .1
	 apex 2 -7 -.8 apex_radius .1 }

//      glass base
cone { apex 2 -7 -.8 apex_radius .1
	 base 2 -7 -.9 base_radius .8 }
cone { apex 2 -7 -.9 apex_radius .8
	 base 2 -7 -.99 base_radius .9 }
ring { center 2 -7 -.99 normal 0 0 -1 radius .9 }

//      fluid in glass

surface {
	transparent 1 .2 .4
	ior 1.3
	shine 20
}

ring { center 2 -7 1.21 normal 0 0 -1 radius .39 }
cone { apex 2 -7 1.21 apex_radius .39
	 base 2 -7 1.3 base_radius .59 }
cone { apex 2 -7 1.3 apex_radius .59
	 base 2 -7 2.5 base_radius .8 }

//      rippled surface of fluid

surface {
	transparent 1 .2 .4
	ior 1.2
	shine 20 white
	bump {
		wave {
			center 2.1 -7 2.5
			wavelength 0.15
			amp .4
			damping 1.0
			phase 0
		}
	}
}

ring { center 2 -7 2.5 normal 0 0 1 radius .8 }


//
//      white chess pieces
//

surf { amb .2 .2 .2   diff .6 .6 .6   spec .5 .5 .5   shine 300 white }

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate 3.5 2.5 0
}
#include pawn.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate 2.5 2.5 0
}
#include pawn.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate 1.5 2.5 0
}
#include pawn.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate 0.5 2.5 0
}
#include pawn.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate -0.5 2.5 0
}
#include pawn.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate -1.5 2.5 0
}
#include pawn.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate -2.5 2.5 0
}
#include pawn.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate -3.5 2.5 0
}
#include pawn.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate 3.5 3.5 0
}
#include rook.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate 2.5 3.5 0
}
#include knight.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate 1.5 3.5 0
}
#include bishop.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate 0.5 3.5 0
}
#include queen.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate -0.5 3.5 0
}
#include king.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate -1.5 3.5 0
}
#include bishop.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate -2.5 3.5 0
}
#include knight.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate -3.5 3.5 0
}
#include rook.bo
transform_pop


//
//      black chess pieces
//

surf { amb .2 .2 .2   diff black   spec .5 .5 .5   shine 300 white }

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate 3.5 -2.5 0
}
#include pawn.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate 2.5 -2.5 0
}
#include pawn.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate 1.5 -2.5 0
}
#include pawn.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate 0.5 -2.5 0
}
#include pawn.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate -0.5 -2.5 0
}
#include pawn.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate -1.5 -2.5 0
}
#include pawn.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate -2.5 -2.5 0
}
#include pawn.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate -3.5 -2.5 0
}
#include pawn.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate 3.5 -3.5 0
}
#include rook.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate 2.5 -3.5 0
}
#include knight.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate 1.5 -3.5 0
}
#include bishop.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate 0.5 -3.5 0
}
#include queen.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate -0.5 -3.5 0
}
#include king.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate -1.5 -3.5 0
}
#include bishop.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate -2.5 -3.5 0
}
#include knight.bo
transform_pop

transform {
	scale 0.001 0.001 0.001
	rotate 90 0 0
	translate -3.5 -3.5 0
}
#include rook.bo
transform_pop
