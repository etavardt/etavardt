/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                Floor3.B = Wood Floor with Platonic Solids               บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                         ALL RIGHTS RESERVED                             บ
บ                                                                         บ
บ   Requires : color.bc, tetra.bo, cube.bo, octa.bo, dodeca.bo, icosa.bo  บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

#define WoodTerms       (3)
#define GrainTerms      (2)

#define LightWood                                               \
surf {                                                          \
	texture {                                               \
		pattern noise                                   \
		scale 1 .01 .01                                 \
		terms GrainTerms                                \
		surf { diff brown shine 100 white }             \
		surf { diff medium_tan shine 100 white }        \
	}                                                       \
}

#define DarkWood                                                \
surf {                                                          \
	texture {                                               \
		pattern noise                                   \
		scale 1 .01 .01                                 \
		terms GrainTerms                                \
		surf { diff dim_brown shine 20 white }         \
		surf { diff dark_tan shine 20 white }    \
	}                                                       \
}

#define Wood                                                    \
surface {                                                       \
	texture {                                               \
		turbulence { scale .1 .9 1 amp .4 terms WoodTerms }     \
		pattern spherical                               \
		scale 0 1 1                                     \
		translate WoodOffset                               \
		blend .5                                        \
		radius .02 radius .05                           \
		DarkWood LightWood                              \
	}                                                       \
}

studio  {
	from 15 -15 6
	at  5 5 0
	up  0 0 1
	angle  35
	res  640 480
	antialias adaptive
	jitter
	aspect  1.3333
	background  black
	ambient  .2 .2 .2
}

//      light sources

light  {
	type point
	falloff 1
	position 15 -5 10
	color 12 12 12
}

light  {
	type point
	falloff 1
	position 5 -10 10
	color 12 12 12
}

//      wood floor boards

#define WoodOffset (0 9 -1)
Wood
polygon { points 4
	vertex 0 10 0
	vertex 3 10 0
	vertex 3 8 0
	vertex 0 8 0
}
#define WoodOffset (-30 9 -.5)
Wood
polygon { points 4
	vertex 3 10 0
	vertex 10 10 0
	vertex 10 8 0
	vertex 3 8 0
}
#define WoodOffset (20 7.5 -1.2)
Wood
polygon { points 4
	vertex 0 8 0
	vertex 10 8 0
	vertex 10 6 0
	vertex 0 6 0
}
#define WoodOffset (-10 5 .8)
Wood
polygon { points 4
	vertex 0 6 0
	vertex 6 6 0
	vertex 6 4 0
	vertex 0 4 0
}
#define WoodOffset (10 5 1.8)
Wood
polygon { points 4
	vertex 6 6 0
	vertex 10 6 0
	vertex 10 4 0
	vertex 6 4 0
}
#define WoodOffset (10 4 -1)
Wood
polygon { points 4
	vertex 0 4 0
	vertex 2 4 0
	vertex 2 2 0
	vertex 0 2 0
}
#define WoodOffset (50 3 1)
Wood
polygon { points 4
	vertex 2 4 0
	vertex 10 4 0
	vertex 10 2 0
	vertex 2 2 0
}
#define WoodOffset (70 2 1.1)
Wood
polygon { points 4
	vertex 0 2 0
	vertex 7 2 0
	vertex 7 0 0
	vertex 0 0 0
}
#define WoodOffset (90 2 .9)
Wood
polygon { points 4
	vertex 7 2 0
	vertex 10 2 0
	vertex 10 0 0
	vertex 7 0 0
}

//      platonic solids on wood floor

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

transform { scale .6 translate 5 5 0 }

transform {  scale 2 translate 0 -3.5 2 }
#include tetra.bo
transform_pop
transform { scale 1.2 rotate 0 0 20 translate 4.5 0 1.2 }
#include cube.bo
transform_pop
transform { scale 2.828 translate 2.5 5 2 }
#include octa.bo
transform_pop
transform { scale 1.236 translate -2.5 5 2 }
#include dodeca.bo
transform_pop
transform { scale 2 translate -4.5 0 2 }
#include icosa.bo
transform_pop

transform_pop

/*

//      reflective walls - these look bad

surf {
	spec .9 .9 .9
}

polygon { points 4
	vertex 0 0 0
	vertex 0 0 25
	vertex 0 10 25
	vertex 0 10 0
}
polygon { points 4
	vertex 0 10 0
	vertex 0 10 25
	vertex 10 10 25
	vertex 10 10 0
}

surf {
	transparent white
	ior 1.0
	spec .9 .9 .9
}

polygon { points 4
	vertex 10 10 0
	vertex 10 10 25
	vertex 10 0 25
	vertex 10 0 0
}
polygon { points 4
	vertex 10 0 0
	vertex 10 0 25
	vertex 0 0 25
	vertex 0 0 0
}

*/
