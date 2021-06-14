/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ   Bkg2.B = Art? - Well - a good illustration of recursion - background  บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                           ALL RIGHTS RESERVED                           บ
บ                                                                         บ
บ                    Requires : color.bc, greys.map                       บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio  {
	from 0 -3 0
	at  0 0 0
	up  0 0 1
	angle  80
	res  640 480
	antialias corners
	projection flat
	aspect  1.3333
	background { greys.map }
}

//      light sources

light  {
	type  point
	falloff  0
	position 2 -3 7
	color  .9 .9 .9
}

//      ground plane

surf {
	texture {
		pattern checker
		scale 1 1 0
		surf { diff plum }
		surf { diff white }
	}
}
/*
ring  {
	center  0 0 0
	normal  0 0 1
	radius  10000
}
*/

//      chrome spheres

surf { spec white shine 1000 }
sphere { center 0 0 0 radius 1 }

surf { spec 1 .5 .5 shine 1000 }
sphere { center 1.2 -.5 1 radius .6 }

surf { spec 1 .5 1 shine 1000 }
sphere { center -.5 -1 -.9 radius .4 }

surf { spec 1 1 .5 shine 1000 }
sphere { center -.8 -1 .6 radius .7 }

surf { spec .5 1 1 shine 1000 }
sphere { center 0 -6 0 radius 2 }

surf { spec .5 1 .5 shine 1000 }
sphere { center .3 -.7 0 radius .1 }

surf { spec .5 .5 1 shine 1000 }
sphere { center 3 -3 -3 radius 4 }

