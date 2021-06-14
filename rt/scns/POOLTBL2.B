/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                PoolTbl2.B = Pool Table and Balls with Cues              บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                            ALL RIGHTS RESERVED                          บ
บ                                                                         บ
บ            Requires : color.bc, greys.map, balls2.bo, chalk.bo,         บ
บ                       stick.bo, boblogo.bo, stripe.bo, solid.bo         บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio {
	from -20 -10 15
	at 6 0 1
	up 0 0 1
	angle 30
	res 640 480
	antialias adaptive
	threshold 2
//	jitter
	aspect 1.333
	background black
	ambient .2 .2 .2
}

//#define TYPE            spherical radius 3 samples 16
//#define LIGHT_COLOR     220 220 220
//light { type TYPE falloff 2 position -5 -5 20 color LIGHT_COLOR }
//light { type TYPE falloff 2 position  5 -5 20 color LIGHT_COLOR }
//light { type TYPE falloff 2 position -5  5 20 color LIGHT_COLOR }
//light { type TYPE falloff 2 position  5  5 20 color LIGHT_COLOR }

#define TYPE            point
#define LIGHT_COLOR    300 300 300
light { type TYPE falloff 2 position -20  10 40 color LIGHT_COLOR }
light { type TYPE falloff 2 position   0  10 40 color LIGHT_COLOR }
light { type TYPE falloff 2 position  20  10 40 color LIGHT_COLOR }
light { type TYPE falloff 2 position -20 -10 40 color LIGHT_COLOR }
light { type TYPE falloff 2 position   0 -10 40 color LIGHT_COLOR }
light { type TYPE falloff 2 position  20 -10 40 color LIGHT_COLOR }


//      felt

surf {
	diff .065 .27 .065      // dark green
	fuzz 0.2
	no_antialias
}
ring { center 0 0 0 normal 0 0 1 radius 1000 }

#include balls2.bo

transform { rotate 0 0 35 translate 1 -4 0 }
#include chalk.bo
transform_pop

transform { rotate 0 -.434 0 translate 0 0 .25 }
#include stick.bo
transform_pop

transform { rotate 0 .5 -30 translate 4 2 .77 }
#include stick.bo
transform_pop

surf { ambient 2.2 0 0 }  // neon red
transform { scale 5 rotate 90 0 -10 translate -50 -25 15 }
#include boblogo2.bo
transform_pop

