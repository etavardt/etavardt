/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                             Bob Ray Tracer                              บ
บ                                                                         บ
บ                    Error.C = error trapping routines			  บ
บ                                                                         บ
บ       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     บ
บ                                                                         บ
บ       ALL RIGHTS RESERVED.   This software is published, but is NOT     บ
บ         Public Domain and remains the propery of ALGORITHM, Inc.,       บ
บ   Christopher D. Watkins and Stephen B. Coy.  This software may not be  บ
บ  reproduced or integrated into other packages without the prior written บ
บ          consent of Christopher D. Watkins and Stephen B. Coy.          บ
บ                                                                         บ
บ                       Requires: defs.h, extern.h                        บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include <stdio.h>
#include <math.h>
#include "defs.h"
#include "extern.h"

/*
 * various routines to print error messages and die...
 */

ObjectProcs NullProcs = {
	NullIntersect,
	NullNormal
};

int     NullIntersect(void)
{
	fprintf(stderr, "Called non-existant intersect routine for bounding box, dying...\n");
	last_call(1);
	return 1;       /* keep lint/compilers quiet */
}

void    NullNormal(void)
{
	fprintf(stderr, "Called non-existant normal routine for bounding box, dying...\n");
	last_call(1);
}



