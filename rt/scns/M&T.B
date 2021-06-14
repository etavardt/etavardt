/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ      M&T.B = Studio File for ED Example: M&T logo by William Tolhurst   บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                          ALL RIGHTS RESERVED                            บ
บ                                                                         บ
บ                      Requires : m.bo, &.bo, t.bo                        บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

studio {
//       from 250 50 100
//       at 0 0 40
       from 250 85 100
       at -90 0 40
       up 0 0 1
       angle 48
       resolution 640 480
       aspect 1.6
       projection flat
       ambient .4 .4 .4
       background .45 .45 .8
       haze .0005
       antialias adaptive
       threshold 4
       jitter
       aperture 0
       focal_length 261.916
       samples 8
       depth 20
}

//      light sources

light { type point falloff 1 position  50 -100 100 color 35 35 35 }
light { type spot position 80 0 180 at 0 30 30 min_angle 15 max_angle 35 color 35 35 35 falloff 1}
light { type point falloff 1 position 50 130 100 color 35 35 35 }

//      ground plane, added manually

surface {
        diffuse .3 .2 .6
}

ring {
     center 0 0 0
     normal 0 0 1
     radius 5000
}


surface {
        diffuse 1 0 0
        ambient 0 0 0
        specular 0 0 0
        shine 20 1 0 0
        transparent 0 0 0
        ior 1.0
        fuzz 0.0
}

transform {
          scale 1
          rotate 0 0 0
          translate 0 -80 0
}
#include m.bo
transform_pop

surface {
        diffuse 0 1 0
        ambient 0 0 0
        specular 0 0 0
        shine 20 0 1 0
        transparent 0 0 0
        ior 1.0
        fuzz 0.0
}

transform {
          scale 1
          rotate 0 0 0
          translate 0 0 0
}
#include &.bo
transform_pop

surface {
        diffuse 1 0 0
        ambient 0 0 0
        specular 0 0 0
        shine 20 1 0 0
        transparent 0 0 0
        ior 1.0
        fuzz 0.0
}

transform {
          scale 1
          rotate 0 0 0
          translate 0 70 0
}
#include t.bo
transform_pop
