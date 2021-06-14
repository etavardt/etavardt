/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ          Box.B = the test for the fisheye (spherical) projection        บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                            ALL RIGHTS RESERVED                          บ
บ                                                                         บ
บ                          Requires : color.bc                            บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio  {
	from  0 0 8
	at  0 0 0
	up  0 0 1
	angle  120
	resolution 640 480
        projection fisheye
	antialias adaptive
	threshold 2
	aspect  1.333
	background  black
	ambient .1 .1 .1
}

//      light source outside of box

light  {
	type  point
	falloff  0
	position  20  -10 40
	color  .8 .8 .8
}

//      light source inside box (light inside a sphere)

//      light body

surf {
	ambient white
	transparent white
}
sphere { center .5 .5 3 radius 0.6 }

//      light inside body

light { type spherical radius .5 samples 2 falloff 2 color 6.5 6.5 6.5 position .5 .5 3 }


//      spheres inside box

surf {
        diffuse .6 0 0
        spec .5 .5 .5
}
sphere { center 2.3 -2.3 1.5 radius 1.5 }


surf {
        diffuse 0 0 .6
        spec .5 .5 .5
}
sphere { center -2.8 -2.8 1.0 radius 1.2 }


//      crystal ball

surf {
	transparent .98 .98 .98
	specular .1 .1 .1
	ior 1.52
	shine 100 white
}
sphere { center -2.1 1.6 2.5 radius 1.5 }


//      table top

surface {
	diff dark_slate_grey
}

ring  { radius 50 center  0 0 -1 normal  0 0 1 }


//      box

// floor

surface  {
	texture  {
		pattern checker
		scale 1 1 0
		surface  { diff gold  shine  20 }
		surface  { diff silver  shine  20 }
	}
}

polygon  {
	points  4
	vertex   4  4 0
	vertex   4 -4 0
	vertex  -4 -4 0
	vertex  -4  4 0
}


// left wall

surface  {
	texture  {
		pattern checker
		scale 0 1 1
		surface  { diff gold  shine  20 }
		surface  { diff silver  shine  20 }
	}
}

polygon  {
	points  4
	vertex   4  4 0
	vertex   4 -4 0
	vertex   4 -4 4
	vertex   4  4 4
}


// right wall

surface  {
	texture  {
		pattern checker
		scale 0 1 1
		surface  { diff gold  shine  20 }
		surface  { diff silver  shine  20 }
	}
}

polygon  {
	points  4
	vertex  -4  4 4
	vertex  -4 -4 4
	vertex  -4 -4 0
	vertex  -4  4 0
}


// bottom wall

surface  {
	texture  {
		pattern checker
		scale 1 0 1
		surface  { diff gold  shine  20 }
		surface  { diff silver  shine  20 }
	}
}

polygon  {
	points  4
	vertex   4  4 4
	vertex  -4  4 4
	vertex  -4  4 0
	vertex   4  4 0
}


// top wall

surface  {
	texture  {
		pattern checker
		scale 1 0 1
		surface  { diff gold  shine  20 }
		surface  { diff silver  shine  20 }
	}
}

polygon  {
	points  4
	vertex   4 -4 4
	vertex   4 -4 0
	vertex  -4 -4 0
	vertex  -4 -4 4
}

