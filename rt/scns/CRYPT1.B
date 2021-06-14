/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ           Crypt1.B = Crypt made of Columns and Clipped Surfaces         บ
บ                                                                         บ
บ          (c)1988,1992 Christopher D. Watkins and Stephen B. Coy         บ
บ                           ALL RIGHTS RESERVED                           บ                                                                         บ
บ                                                                         บ
บ     Requires : color.bc, greys.map, crypt.bo, cryptsec.bo, column.bo    บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include color.bc

studio {
	from 84 -144 60
	at 36 0 48
	up 0 0 1
	angle 70
	res 640 480
	antialias adaptive
	threshold 2
	jitter
	aspect 1.333
	background {
		greys.map
	}
}

#include crypt.bo
