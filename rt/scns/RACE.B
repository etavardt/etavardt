/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ           Race.B = Hot-Rod and Corvette at Finish Line                  บ
บ                        (Hot-Rod and Corvette models by Mira Imaging)    บ
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
	from 4 12 10
	at 4 0 -3
	up 0 0 1
	angle 60
	res 640 480
	antialias adaptive
	threshold 2
	aspect 1.0
	background {
		greys.map
		up 0 0 -1
	}
	ambient .1 .1 .1
}

light {
	type point
	falloff 0
	position 1 -.5 5
	color .5 .5 .5
}

light {
	type point
	falloff 0
	position -2 -4 5
	color .5 .5 .5
}

//      Hot-Rod

surface {
	diffuse steel_blue
	spec .3 .3 .3
	shine 400 white
}

#include hotrod.bo

//      Corvette

surface {
	diffuse red
	spec .3 .3 .3
	shine 50 white
}

transform {
	rotate 0 0 90
	translate 9 0 -1
}
#include vette.bo
transform_pop


// 	Finish Line

surf {
	texture {
		pattern checker
		scale 2 2 1
		surf { diff .5 .5 .5 spec .5 .5 .5 }
		surf { diff 0 0 0 spec 0 0 0 }
	}
}

polygon {
     points  4
     vertex  -1000 -10 0
     vertex   1000 -10 0
     vertex   1000 -14 0
     vertex  -1000 -14 0
}
