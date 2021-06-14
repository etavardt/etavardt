/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ              Gallery.B = Art Gallery - Use of many bit maps             บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                           ALL RIGHTS RESERVED                           บ
บ                                                                         บ
บ    Requires : color.bc, greys.map, column2.bo, ccdsatrn.img, cnn.img    บ
บ                     palermo.img, planets2.img, shuttle.img              บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio {
	from 100 -150 120
	at 30 0 30
	up 0 0 1
	angle 75
	resolution 640 480
	antialias adaptive
	threshold 2
	aspect 1.3333

	ambient .2 .2 .2
}


//light {
//	type spherical
//	samples 32
//	radius 10
//	falloff 0
//	position 100 -100 200
//	color .9 .9 .9
//}

light {
	type point
	falloff 0
	position 100 -100 200
	color 1 1 1
}


//      floor

transform { scale 6 }
surface {       //      marble, white with dark blue veins
	texture {
		pattern spherical
		turbulence {
			terms 7
			amp 6
		}
		blend .8
		radius .5
		radius 2
		surf { diff .1 .15 .25  shine 20 white }
		surf { diff .8 .8 .8 shine 20 white }
	}
}
transform_pop
ring { center 0 0 0 normal 0 0 1 radius 1000 }


//      left wall

surface {
	diff .7 .6 .5
}
ring { center 0 0 0 normal 1 0 0 radius 1000 }


//      right wall

surface {
	diff .9 .8 .7
}
ring { center 0 0 0 normal 0 1 0 radius 1000 }


//      corner shelf made of glass with chrome edge

surf {
	transparent .8 .8 .8
	specular .1 .1 .1
	ior 1.6
	shine 100 white
}
ring { center 0 0 47 normal 0 0 1 radius 40 }
ring { center 0 0 45 normal 0 0 -1 radius 40 }

// chrome edge
surf {
       ambient  .1 .1 .1
       diffuse  .3 .3 .3
       specular .4 .4 .4
       shine 100 white
}
cone { apex 0 0 47 base 0 0 45 radius 40 }


//      column holding up the glass table top

surface {
        ambient .1 .1 .1
	diff .1 .1 .4
}

transform { translate 80 -80 0 }
#include column2.bo
transform_pop


//      the glass table top with chrome edge

surf {
	transparent .8 .8 .8
	specular .1 .1 .1
	ior 1.52
	shine 100 white
}
ring { center 80 -80 50 normal 0 0 1 radius 30 }    // top of glass
ring { center 80 -80 48 normal 0 0 -1 radius 30 }   // bottom of glass

// chrome edge
surf {
       ambient  .1 .1 .1
       diffuse  .3 .3 .3
       specular .4 .4 .4
       shine 100 white
}
cone { apex 80 -80 50 base 80 -80 48 radius 30 }


//      left most painting on right wall

#define LL  26 -1  50
#define UL  26 -1 102
#define UR  90 -1 102
#define LR  90 -1  50

// frame
surface {
	diff black
	shine 10 white
}
sphere { center LL radius 1 }
sphere { center UL radius 1 }
sphere { center UR radius 1 }
sphere { center LR radius 1 }
cone { apex LL base UL radius 1 }
cone { apex UL base UR radius 1 }
cone { apex UR base LR radius 1 }
cone { apex LR base LL radius 1 }

// painting
surface {
	diff {
		image palermo.img
		position UL
		across 1 0 0
		normal 0 -1 0
		scale 64
	}
}
polygon { points 4
	vertex LL
	vertex UL
	vertex UR
	vertex LR
}

#undef LR
#undef UR
#undef UL
#undef LL


//      right most painting on left wall

#define LL 1 -100  50
#define UL 1 -100 102
#define UR 1  -36 102
#define LR 1  -36  50

// frame
surface {
	diff black
	shine 10 white
}
sphere { center LL radius 1 }
sphere { center UL radius 1 }
sphere { center UR radius 1 }
sphere { center LR radius 1 }
cone { apex LL base UL radius 1 }
cone { apex UL base UR radius 1 }
cone { apex UR base LR radius 1 }
cone { apex LR base LL radius 1 }

// painting
surface {
	diff {
		image planets2.img
		position UL
		across 0 1 0
		normal 1 0 0
		scale 64
	}
}
polygon { points 4
	vertex LL
	vertex UL
	vertex UR
	vertex LR
}

#undef LR
#undef UR
#undef UL
#undef LL


//      right most painting on right wall

#define LL 110 -1  50
#define UL 110 -1 102
#define UR 174 -1 102
#define LR 174 -1  50

// frame
surface {
	diff black
	shine 10 white
}
sphere { center LL radius 1 }
sphere { center UL radius 1 }
sphere { center UR radius 1 }
sphere { center LR radius 1 }
cone { apex LL base UL radius 1 }
cone { apex UL base UR radius 1 }
cone { apex UR base LR radius 1 }
cone { apex LR base LL radius 1 }

// painting
surface {
	diff {
		image shuttle.img
		position UL
		across 1 0 0
		normal 0 -1 0
		scale 64
	}
}
polygon { points 4
	vertex LL
	vertex UL
	vertex UR
	vertex LR
}

#undef LR
#undef UR
#undef UL
#undef LL


//      right most unmounted painting on glass table

#define LL  80 -80 51.5
#define UL  80 -64 51.5
#define UR  96 -64 51.5
#define LR  96 -80 51.5

// painting
surface {
	diff {
		image ccdsatrn.img
		position UL
		across 1 0 0
		normal 0 0 1
		scale 16
	}
}
polygon { points 4
	vertex LL
	vertex UL
	vertex UR
	vertex LR
}

#undef LR
#undef UR
#undef UL
#undef LL


//      left most unmounted painting on glass table

#define LL  80 -104 51.5
#define UL  60 -104 51.5
#define UR  60  -88 51.5
#define LR  80  -88 51.5

// painting
surface {
	diff {
		image cnn.img
		position UL
		across 0 1 0
		normal 0 0 1
		scale 16
	}
}
polygon { points 4
	vertex LL
	vertex UL
	vertex UR
	vertex LR
}

#undef LR
#undef UR
#undef UL
#undef LL
