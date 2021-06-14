/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                    Lakeside.B = Rush's Lakeside Park                    บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                          ALL RIGHTS RESERVED                            บ
บ                                                                         บ
บ         Requires : color.bc, greys.map, trees.bo, algorith.img          บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

     note : trees > trees.bo
*/

#include color.bc

studio {
	from 0 -300 100
	at 0 0 60
	up 0 0 1
	angle 40
	resolution 640 480
	aspect  1.3333
	antialias adaptive
	threshold 4
        haze 0.0003
	background {
          sky.map
        }
	ambient .1 .1 .1
}

//      the sun

light { type point falloff 0 position -200 -300 500 color .5 .5 .5 }


//      grass

surface { diffuse forest_green }
ring { center 0 0 0 normal 0 0 1 radius 10000 }


//      lake

surface { ambient  .1 .1 .2
          diffuse  .1 .2 .2
          specular .1 .1 .3
}
ring { center 0 5000 1 normal 0 0 1 radius 4750 }


//      lotsa shrubberies

#include trees.bo

transform { rotate 0 0 45
            translate 50 -4 0
          }
#include trees.bo
transform_pop

transform { rotate 0 0 132
            translate -75 20 0
          }
#include trees.bo
transform_pop

transform { rotate 0 0 223
            translate -40 90 0
          }
#include trees.bo
transform_pop

transform { rotate 0 0 331
            translate 43 90 0
          }
#include trees.bo
transform_pop

transform { rotate 0 0 41
            translate 95 45 0
          }
#include trees.bo
transform_pop

transform { rotate 0 0 25
            translate -94 64 0
          }
#include trees.bo
transform_pop

transform { rotate 0 0 25
            translate -115 80 0
          }
#include trees.bo
transform_pop


//        sign

surface { diffuse brown }
cone { apex 60 -50 0 base 60 -50 40 radius 3 }

surface {
	diff {
		image algorith.img
		position 35 -53 50.5
		across 1 0 0
		normal 0 -1 0
		scale 50
	}
}

polygon { points 4
   	  vertex  35 -53  29.5
	  vertex  35 -53  50.5
	  vertex  85 -53  50.5
          vertex  85 -53  29.5
}
