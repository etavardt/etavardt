/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ          Progrs1B.B = Progressive Image Refinement                      บ
บ                         diffuse shading                                 บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                          ALL RIGHTS RESERVED                            บ
บ                                                                         บ
บ             Requires : color.bc, column2.bo, sculpt.bo                  บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio {
	from 60 -100 90
	at 30 0 30
	up 0 0 1
	angle 70
	resolution 640 480
	antialias adaptive
	threshold 2
	aspect 1.3333

	ambient .2 .2 .2
	no_shadows
}

light {
	type point
	falloff 0
	position 100 -100 200
	color .9 .9 .9
}

//      floor

transform { rotate 0 0 45 }
surface {
	diff .8 .8 .8
}
transform_pop
ring { center 0 0 0 normal 0 0 1 radius 1000 }

//      brick wall on left

surface {
	diff firebrick
}
ring { center 0 0 0 normal 1 0 0 radius 1000 }

//      back wall

surface {
	diff .9 .8 .7
}
ring { center 0 0 0 normal 0 1 0 radius 1000 }

//      marble column with ball scuplture

surface {
	diff white
}

transform { translate 24 -24 0 }
#include column2.bo
transform_pop

//      sculpture

surface {       // metal surface
	amb 0.035 0.0325 0.025
	diffuse 0.5 0.45 0.35
}

transform { scale 24 translate 24 -24 59 }
#include sculpt.bo
transform_pop

//      marble column with crystal ball

surface {
	diff .03 .15 .03
}

transform { translate 72 -36 0 }
#include column2.bo
transform_pop

//      crystal ball

surf {
}
sphere { center 72 -36 59 radius 12 }

//      painting on back wall
//      frame

surface {
	diff black
}
sphere { center 36 -1 36 radius 1 }
sphere { center 36 -1 72 radius 1 }
sphere { center 84 -1 72 radius 1 }
sphere { center 84 -1 36 radius 1 }
cone { apex 36 -1 36 base 36 -1 72 radius 1 }
cone { apex 36 -1 72 base 84 -1 72 radius 1 }
cone { apex 84 -1 72 base 84 -1 36 radius 1 }
cone { apex 84 -1 36 base 36 -1 36 radius 1 }

surface {
	diff sky_blue
}

polygon { points 4
	vertex 36 -1 36
	vertex 36 -1 72
	vertex 84 -1 72
	vertex 84 -1 36
}

