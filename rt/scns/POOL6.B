/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                     Pool6.B = Swimming Pool Ladder                      บ
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
	from 20 100 50
	at 60 10 -10
	up 0 0 1
	angle 45
	res 640 480
	antialias adaptive
	aspect 1.3333

	background sky_blue
	ambient .2 .2 .2

	no_exp_trans
}

light { type point falloff 0 color .7 .7 .7 position -200 -300 600 }
light { type point falloff 0 color .7 .7 .7 position 800 1000 300 }

//      water

surface {
	transparent .7 .9 .9
	ior 1.3
	spec .2 .2 .2
	shine 20 .7 .9 .9
	bump {
		wave {
			center 200 300 10
			wavelength 30
			amp .3
		}
		wave {
			center 300 100 10
			wavelength 17
			amp .2
		}
		wave {
			center 600 500 10
			wavelength 9
			amp .1
		}
	}
}

polygon {
	points 4
	vertex -1 -1 -14
	vertex -1 400 -14
	vertex 600 400 -14
	vertex 600 -1 -14
}

//      tile surface

transform { translate 5.755 5.755 5.755 }
surface {
	texture {
		pattern spherical
		scale 1 0 0
		radius 8.5
		radius 1.5
		surface {
			texture {
				pattern spherical
				scale 0 1 0
				radius 8.5
				radius 1.5
				surface {
					texture {
						pattern spherical
						scale 0 0 1
						radius 8.5
						radius 1.5
						surf { diff sky_blue shine 30 1 1 1 }
						surf { diff medium_tan }
					}
				}
				surf { diff medium_tan }
			}
		}
		surf { diff medium_tan }
	}
}
transform_pop

//      deck tiles
polygon {
	points 4
	vertex -1 -1 0
	vertex 600 -1 0
	vertex 636 -37 0
	vertex -37 -37 0
}
polygon {
	points 4
	vertex -1 -1 0
	vertex -37 -37 0
	vertex -37 436 0
	vertex -1 400 0
}
polygon {
	points 4
	vertex -1 400 0
	vertex -37 436 0
	vertex 636 436 0
	vertex 600 400 0
}
polygon {
	points 4
	vertex 600 -1 0
	vertex 600 400 0
	vertex 636 436 0
	vertex 636 -37 0
}

//      rounds on outer edges
sphere { center -37 -37 -1 radius 1 }
sphere { center -37 436 -1 radius 1 }
sphere { center 636 -37 -1 radius 1 }
sphere { center 636 436 -1 radius 1 }
cone { apex -37 -37 -1 base -37 436 -1 apex_radius 1 base_radius 1 }
cone { apex -37 -37 -1 base 636 -37 -1 apex_radius 1 base_radius 1 }
cone { apex 636 436 -1 base -37 436 -1 apex_radius 1 base_radius 1 }
cone { apex 636 436 -1 base 636 -37 -1 apex_radius 1 base_radius 1 }

//      vertical inner tiles
polygon {
	points 4
	vertex 0 0 -1
	vertex 0 399 -1
	vertex 0 399 -27
	vertex 0 0 -27
}
polygon {
	points 4
	vertex 0 399 -1
	vertex 599 399 -1
	vertex 599 399 -27
	vertex 0 399 -27
}
polygon {
	points 4
	vertex 599 399 -1
	vertex 599 0 -1
	vertex 599 0 -27
	vertex 599 399 -27
}
polygon {
	points 4
	vertex 599 0 -1
	vertex 0 0 -1
	vertex 0 0 -27
	vertex 599 0 -27
}


//      top rounds on inner edges
cone { apex -1 -1 -1 base -1 400 -1 apex_radius 1 base_radius 1 }
cone { apex -1 -1 -1 base 600 -1 -1 apex_radius 1 base_radius 1 }
cone { apex 600 -1 -1 base 600 400 -1 apex_radius 1 base_radius 1 }
cone { apex -1 400 -1 base 600 400 -1 apex_radius 1 base_radius 1 }

//      bottom rounds on inner edges
cone { apex -1 -1 -27 base -1 400 -27 apex_radius 1 base_radius 1 }
cone { apex -1 -1 -27 base 600 -1 -27 apex_radius 1 base_radius 1 }
cone { apex 600 -1 -27 base 600 400 -27 apex_radius 1 base_radius 1 }
cone { apex -1 400 -27 base 600 400 -27 apex_radius 1 base_radius 1 }

//      pool walls and bottom
surface {
	diff .9 .9 .9
}
polygon {
	points 4
	vertex -1 -1 -27
	vertex -1 400 -27
	vertex -1 400 -200
	vertex -1 -1 -200
}
polygon {
	points 4
	vertex -1 400 -27
	vertex 600 400 -27
	vertex 600 400 -200
	vertex -1 400 -200
}
polygon {
	points 4
	vertex 600 400 -27
	vertex 600 -1 -27
	vertex 600 -1 -200
	vertex 600 400 -200
}
polygon {
	points 4
	vertex 600 -1 -27
	vertex -1 -1 -27
	vertex -1 -1 -200
	vertex 600 -1 -200
}
//      pool bottom
polygon {
	points 4
	vertex -1 -1 -200
	vertex -1 400 -200
	vertex 600 400 -200
	vertex 600 -1 -200
}

//      chrome ladder
surface {
	spec .7 .7 .7
	shine 50
}
//      joints
sphere { center 50 -20  20 radius 2.0 }
sphere { center 50 -10  30 radius 2.0 }
sphere { center 50   0  30 radius 2.0 }
sphere { center 50  10  20 radius 2.0 }
sphere { center 50  10 -60 radius 2.0 }
sphere { center 80 -20  20 radius 2.0 }
sphere { center 80 -10  30 radius 2.0 }
sphere { center 80   0  30 radius 2.0 }
sphere { center 80  10  20 radius 2.0 }
sphere { center 80  10 -60 radius 2.0 }
//      tubes
cone { apex 50 -20  0 base 50 -20 20 apex_radius 2.0 base_radius 2.0 }
cone { apex 50 -20 20 base 50 -10 30 apex_radius 2.0 base_radius 2.0 }
cone { apex 50 -10 30 base 50   0 30 apex_radius 2.0 base_radius 2.0 }
cone { apex 50   0 30 base 50  10 20 apex_radius 2.0 base_radius 2.0 }
cone { apex 50  10 20 base 50  10 -60 apex_radius 2.0 base_radius 2.0 }
cone { apex 50  10 -60 base 50 -1 -60 apex_radius 2.0 base_radius 2.0 }
cone { apex 80 -20  0 base 80 -20 20 apex_radius 2.0 base_radius 2.0 }
cone { apex 80 -20 20 base 80 -10 30 apex_radius 2.0 base_radius 2.0 }
cone { apex 80 -10 30 base 80   0 30 apex_radius 2.0 base_radius 2.0 }
cone { apex 80   0 30 base 80  10 20 apex_radius 2.0 base_radius 2.0 }
cone { apex 80  10 20 base 80  10 -60 apex_radius 2.0 base_radius 2.0 }
cone { apex 80  10 -60 base 80 -1 -60 apex_radius 2.0 base_radius 2.0 }
//      flush mounts
cone { apex 50 -20  1 base 50 -20 0 apex_radius 2.0 base_radius 4.0 }
cone { apex 50  0 -60 base 50 -1 -60 apex_radius 2.0 base_radius 4.0 }
cone { apex 80 -20  1 base 80 -20 0 apex_radius 2.0 base_radius 4.0 }
cone { apex 80  0 -60 base 80 -1 -60 apex_radius 2.0 base_radius 4.0 }

//      ladder rungs
ring { center 50 10 -10 normal 0 0  1 radius 3 }
ring { center 50 10 -11 normal 0 0 -1 radius 3 }
ring { center 80 10 -10 normal 0 0  1 radius 3 }
ring { center 80 10 -11 normal 0 0 -1 radius 3 }
cone { apex 50 10 -10 base 50 10 -11 apex_radius 3 base_radius 3 }
cone { apex 80 10 -10 base 80 10 -11 apex_radius 3 base_radius 3 }
polygon { points 4 vertex 50 13 -10 vertex 80 13 -10 vertex 80 7 -10 vertex 50 7 -10 }
polygon { points 4 vertex 50 13 -11 vertex 50 7 -11 vertex 80 7 -11 vertex 80 13 -11}
polygon { points 4 vertex 50 13 -10 vertex 50 13 -11 vertex 80 13 -11 vertex 80 13 -10 }
polygon { points 4 vertex 50 7 -10 vertex 50 7 -11 vertex 80 7 -11 vertex 80 7 -10 }

ring { center 50 10 -30 normal 0 0  1 radius 3 }
ring { center 50 10 -31 normal 0 0 -1 radius 3 }
ring { center 80 10 -30 normal 0 0  1 radius 3 }
ring { center 80 10 -31 normal 0 0 -1 radius 3 }
cone { apex 50 10 -30 base 50 10 -31 apex_radius 3 base_radius 3 }
cone { apex 80 10 -30 base 80 10 -31 apex_radius 3 base_radius 3 }
polygon { points 4 vertex 50 13 -30 vertex 80 13 -30 vertex 80 7 -30 vertex 50 7 -30 }
polygon { points 4 vertex 50 13 -31 vertex 50 7 -31 vertex 80 7 -31 vertex 80 13 -31}
polygon { points 4 vertex 50 13 -30 vertex 50 13 -31 vertex 80 13 -31 vertex 80 13 -30 }
polygon { points 4 vertex 50 7 -30 vertex 50 7 -31 vertex 80 7 -31 vertex 80 7 -30 }

ring { center 50 10 -50 normal 0 0  1 radius 3 }
ring { center 50 10 -51 normal 0 0 -1 radius 3 }
ring { center 80 10 -50 normal 0 0  1 radius 3 }
ring { center 80 10 -51 normal 0 0 -1 radius 3 }
cone { apex 50 10 -50 base 50 10 -51 apex_radius 3 base_radius 3 }
cone { apex 80 10 -50 base 80 10 -51 apex_radius 3 base_radius 3 }
polygon { points 4 vertex 50 13 -50 vertex 80 13 -50 vertex 80 7 -50 vertex 50 7 -50 }
polygon { points 4 vertex 50 13 -51 vertex 50 7 -51 vertex 80 7 -51 vertex 80 13 -51}
polygon { points 4 vertex 50 13 -50 vertex 50 13 -51 vertex 80 13 -51 vertex 80 13 -50 }
polygon { points 4 vertex 50 7 -50 vertex 50 7 -51 vertex 80 7 -51 vertex 80 7 -50 }

//      diving board

//      joints
sphere { center 620 180 10 radius 2 }
sphere { center 630 180 20 radius 2 }
sphere { center 650 180 20 radius 2 }
sphere { center 660 180 10 radius 2 }
sphere { center 620 220 10 radius 2 }
sphere { center 630 220 20 radius 2 }
sphere { center 650 220 20 radius 2 }
sphere { center 660 220 10 radius 2 }
//      rails
cone { apex 620 180  0 base 620 180 10 apex_radius 2 base_radius 2 }
cone { apex 620 180 10 base 630 180 20 apex_radius 2 base_radius 2 }
cone { apex 630 180 20 base 650 180 20 apex_radius 2 base_radius 2 }
cone { apex 650 180 20 base 660 180 10 apex_radius 2 base_radius 2 }
cone { apex 660 180 10 base 660 180 -1 apex_radius 2 base_radius 2 }
cone { apex 620 220  0 base 620 220 10 apex_radius 2 base_radius 2 }
cone { apex 620 220 10 base 630 220 20 apex_radius 2 base_radius 2 }
cone { apex 630 220 20 base 650 220 20 apex_radius 2 base_radius 2 }
cone { apex 650 220 20 base 660 220 10 apex_radius 2 base_radius 2 }
cone { apex 660 220 10 base 660 220 -1 apex_radius 2 base_radius 2 }
//      cross bars
cone { apex 620 180 10 base 620 220 10 apex_radius 2 base_radius 2 }
cone { apex 660 180 10 base 660 220 10 apex_radius 2 base_radius 2 }
//      flush mounts
cone { apex 620 180 1 base 620 180 0 apex_radius 2.0 base_radius 4.0 }
cone { apex 620 220 1 base 620 220 0 apex_radius 2.0 base_radius 4.0 }
cone { apex 660 180 -1 base 660 180 -2 apex_radius 2.0 base_radius 4.0 }
cone { apex 660 220 -1 base 660 220 -2 apex_radius 2.0 base_radius 4.0 }
//      bolt heads
sphere { center 660 190 14.7 radius .5 }
sphere { center 660 210 14.7 radius .5 }

//      the board itself
surface { diff .9 .9 .9 }
polygon {                     // top
	points 4
	vertex 500 183 15
	vertex 500 217 15
	vertex 670 217 15
	vertex 670 183 15
}
polygon {                     // bottom
	points 4
	vertex 500 183 12
	vertex 670 183 12
	vertex 670 217 12
	vertex 500 217 12
}
polygon {                     // nose
	points 4
	vertex 500 183 15
	vertex 500 183 12
	vertex 500 217 12
	vertex 500 217 15
}
polygon {                     // tail
	points 4
	vertex 670 183 15
	vertex 670 217 15
	vertex 670 217 12
	vertex 670 183 12
}
polygon {                     // left rail
	points 4
	vertex 500 183 15
	vertex 670 183 15
	vertex 670 183 12
	vertex 500 183 12
}
polygon {                     // right rail
	points 4
	vertex 500 217 15
	vertex 500 217 12
	vertex 670 217 12
	vertex 670 217 15
}


// concrete pool surround
surface { diff .6 .6 .6 }
polygon {
	points 10
	vertex -37 -37 -2
	vertex -5000 -5000 -2
	vertex -5000  5000 -2
	vertex  5000  5000 -2
	vertex  5000 -5000 -2
	vertex -5000 -5000 -2
	vertex -37 -37 -2
	vertex 636 -37 -2
	vertex 636 436 -2
	vertex -37 436 -2
}

