/*
�������������������������������������������
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�              Memory.C = Bob memory functions to replace malloc()        �
�                                                                         �
�       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     �
�                                                                         �
�       ALL RIGHTS RESERVED.   This software is published, but is NOT     �
�         Public Domain and remains the propery of ALGORITHM, Inc.,       �
�   Christopher D. Watkins and Stephen B. Coy.  This software may not be  �
�  reproduced or integrated into other packages without the prior written �
�          consent of Christopher D. Watkins and Stephen B. Coy.          �
�                                                                         �
�                 Requires: defs.h, extern.h, proto.h                     �
�                                                                         �
�������������������������������������������
*/

#include <cstdlib>
#include <cstdio>
#include "defs.hpp"
#include "extern.hpp"
#include "proto.hpp"

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
		ptr = (char *)malloc(CHUNK);
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


