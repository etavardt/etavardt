/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                    Blend1.B = Blending, Turbulence                      บ
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
	from 10 -10 12
	at 3 4 2
	up 0 0 1
	angle 40
	res 640 480
	antialias adaptive
	aspect 1.3333
	background {
		greys.map
	}
	ambient .2 .2 .2

	bunching 8
}

light {
	type point
	falloff 0
	position 1 -3 10
	color .6 .6 .6
}
light {
	type point
	falloff 0
	position 13 0 10
	color .6 .6 .6
}

//      base plane

surf { diff dark_plum }
ring { center 0 0 0 normal 0 0 1 radius 1000 }

//      checkerboard surface

surf {
	texture {
		pattern checker
		scale 1.01 1.01 0
		translate .04 .04 0
		blend .6
		turb {
			scale .6 .6 .6
			terms 6
			amp 6
		}
		surf { diff white shine 50 white }
		surf { diff black shine 50 white }
	}
}
polygon { points 4
	vertex 0 0 .5
	vertex 0 8 .5
	vertex 8 8 .5
	vertex 8 0 .5
}
polygon { points 4
	vertex 0 0 0
	vertex 0 0 .5
	vertex 8 0 .5
	vertex 8 0 0
}
polygon { points 4
	vertex 8 0 0
	vertex 8 0 .5
	vertex 8 8 .5
	vertex 8 8 0
}
polygon { points 4
	vertex 8 8 0
	vertex 8 8 .5
	vertex 0 8 .5
	vertex 0 8 0
}
polygon { points 4
	vertex 0 8 0
	vertex 0 8 .5
	vertex 0 0 .5
	vertex 0 0 0
}

//      cylinder off to the side

surf {
	texture {
		pattern checker
		scale 0 0 .5
		blend 1.0
		turb {
			terms 4
			amp 4
		}
		surf { diff white  shine 50 white }
		surf { diff 0 .5 1 shine 50 white }
	}
}

cone { base -2 4 0 apex -2 4 4 radius 1.5 }
sphere { center -2 4 4 radius 1.5 }

//      chrome/checkered sphere

surf {
	texture {
		pattern checker
		scale 1 1 1
                blend .5
		turb {
			scale .3 .3 .3
			terms 2
			amp 4
		}
		surf { diff .3 .3 .3 spec .7 .7 .7 shine 500 }
		surf { diff dark_brown spec medium_brown shine 500 }
	}
}
sphere { center 3 6 3 radius 2 }

