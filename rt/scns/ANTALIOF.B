/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                  AntAliOf.B = Anti-aliasing test - off                  บ
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
	from 0 -20 50
	at 100 0 20
	up 0 0 1
	angle 50
	resolution 320 200
	// antialias adaptive
	// threshold 1
	// jitter
	aspect 1.3333
	background {
		greys.map
	}

	ambient .2 .2 .2
}

light {
	type point
	falloff 0
	position 100 -500 1000
	color .9 .9 .9
}

//      diagonal checkerboard floor

transform { rotate 0 0 45 }
surface {
	texture {
		pattern checker
		scale 12 12 0
		surface { diff white }
		surface { diff black }
	}
}
transform_pop
ring { center 0 0 0 normal 0 0 1 radius 10000 }

//      big ugly brick texture

#define BRICK   surf { diff firebrick fuzz .1 }
#define MORTAR  surf { diff .6 .6 .6 fuzz .4 }

surf {
	texture {
		pattern checker
		scale 0 4.5 3.5
surf {
	texture {
		pattern spherical
		scale 0 0 1
		radius .5
		radius 3
		MORTAR
		surf {
			texture {
				pattern spherical
				scale 1 0 0
				radius .5
				radius 8
				translate 4.5 0 0
				MORTAR
				surf {
					texture {
						pattern spherical
						scale 0 1 0
						radius .5
						radius 4
						MORTAR
						BRICK
					}
				}
			}
		}
	}
}

surf {
	texture {
		pattern spherical
		scale 0 0 1
		radius .5
		radius 3
		MORTAR
		surf {
			texture {
				pattern spherical
				scale 1 0 0
				radius .5
				radius 8
				MORTAR
				surf {
					texture {
						pattern spherical
						scale 0 1 0
						radius .5
						radius 4
						MORTAR
						BRICK
					}
				}
			}
		}
	}
}

}}

polygon { points 4
	vertex -10000 10 0
	vertex -10000 10 60
	vertex  10000 10 60
	vertex  10000 10 0
}

//      chrome sphere

surface {
	specular white
	shine 100 white
}
sphere { center 100 -20 20 radius 20 }
