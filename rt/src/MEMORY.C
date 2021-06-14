/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                             Bob Ray Tracer                              บ
บ                                                                         บ
บ              Memory.C = Bob memory functions to replace malloc()	  บ
บ                                                                         บ
บ       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     บ
บ                                                                         บ
บ       ALL RIGHTS RESERVED.   This software is published, but is NOT     บ
บ         Public Domain and remains the propery of ALGORITHM, Inc.,       บ
บ   Christopher D. Watkins and Stephen B. Coy.  This software may not be  บ
บ  reproduced or integrated into other packages without the prior written บ
บ          consent of Christopher D. Watkins and Stephen B. Coy.          บ
บ                                                                         บ
บ                 Requires: defs.h, extern.h, proto.h                     บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include <stdlib.h>
#include <stdio.h>
#include "defs.h"
#include "extern.h"
#include "proto.h"

#define CHUNK   (32750)         /* just under 32K */

void    *vmalloc(int size)
{
	static char             *ptr;
	void                    *tmp;
	static unsigned int     amount = 0;

	if(size > CHUNK) {      /* need a big one */
		tmp = malloc(size);
		if(tmp) {
			MemAllocated += size;
		}
		return tmp;     /* may be NULL if not available */
	} else if(size <= amount) {
		tmp = ptr;
		ptr += size;
		amount -= size;
		MemAllocated += size;
		return tmp;
	} else {
		ptr = malloc(CHUNK);
		if(!ptr) {      /* nothing big left so fill in the holes */
			tmp = malloc(size);
			if(tmp) {
				MemAllocated += size;
			}
			return tmp;
		} else {
			amount = CHUNK;
		}
		return vmalloc(size);
	}

}       /* end of vmalloc() */


