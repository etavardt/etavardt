/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ             Ice2.B = Floating Ice Cubes (model by Ron Lloyd)            บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                           ALL RIGHTS RESERVED                           บ
บ                                                                         บ
บ                   Requires : color.bc, sky.map, ice.bo                  บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/


#include color.bc

studio {
    from 6 -8 5
    at 1 0 -1.5
    up 0 0 1
    angle 70
    res 640 480
    antialias adaptive
    aspect 1.333
    background {sky.map}
    depth 3
    ambient .25 .25 .25
    jitter

}

//      light source

light {
    type point
    falloff 1
    position 10 -50 40
    color 40 40 40
}

//      water

surface {
    spec .7 .85 .95
    shine 30
    transparent .99 .85 .9
    ior 1.2
    ambient 0 .04 .05
    bump {
        wave {
            center 0 0 -1
            wavelength 2
            amplitude .09
             }
        wave {
            center 2.75 -.5 -1
            wavelength 3
            amplitude .1
             }
        wave {
            center 1 -2.5 -1
            wavelength .82
            amplitude .15
             }
        turbulence {
            scale 1 1 1
            amplitude .25
            terms 2
                   }

         }
}

ring {
    center 0 0 -1 normal 0 0 1 radius 1000
}


//      ice cubes

surface {
    spec .7 .85 .9
    shine 30
    transparent 1 .975 .975
    ior 1.21
    ambient 0 .05 .05
}

transform {
    rotate 20 0 0
    translate 0 .75 -.75
}
#include ice.bo
transform_pop

transform {
    rotate 30 45 70
    translate 2.75 -.5 -.5
}
#include ice.bo
transform_pop

transform {
    rotate 45 -160 -30
    translate 1 -2.5 1
}
#include ice.bo
transform_pop
