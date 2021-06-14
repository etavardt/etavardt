/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ   RevSol2.B = Quaternion Solid of Revolution (Computed Front to Back)   บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                            ALL RIGHTS RESERVED                          บ
บ                                                                         บ
บ                Requires : color.bc, greys.map, revsol2.bo               บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio {
	from 32 32 1000
	at 32 32 150
	up 0 0 1
	angle 4.3
	res 640 480
	antialias adaptive
	aspect 1.333
	ambient .1 .1 .1

        depth 1
}

light {
	type point
	position 0 200 500
	color .5 .4 .4
}

light {
	type point
	position 200 0 500
	color .4 .4 .5
}

//

surface {
	diffuse silver
	spec .4 .4 .4
	shine 80 white
}

#include revsol2.bo
