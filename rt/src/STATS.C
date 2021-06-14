/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                             Bob Ray Tracer                              บ
บ                                                                         บ
บ      Stats.C = Display info about current trace in progress.  Note      บ
บ		      that this assumes the use of ansi.sys.		  บ
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
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "defs.h"
#include "extern.h"

#define ESC     (27)

void    statistics(int line)
{
	static int      first_call = 1;

	if(first_call) {
		if(tickflag) {
			printf("%c[2J", ESC);   /* clear screen 1st time around */
		} else {
			printf("\n");
		}
		first_call = 0;
	}

	printf("%c[1;1f", ESC);         /* home cursor */

	printf("\n\t\t%s\t\t       %s\n\t\t%s\n", _Program, _Version, _Copyright);

	printf("\ninput file \"%s\"  memory %lu  resolution %1dx%1d  ", Infilename, MemAllocated, Xresolution, Yresolution);
	printf("line %1d\n", line);

	printf("total rays cast\t\t%8lu\n", nRays);
	printf("\teye rays\t%8lu\n", nRays-nReflected-nRefracted);
	printf("\treflected rays\t%8lu\n", nReflected);
	printf("\trefracted rays\t%8lu\n\n", nRefracted);

	printf("shadow rays\t\t%8lu\n", nShadows);
	printf("cache hits\t\t%8lu\n", nShadowCacheHits);
	if(nShadows>0) {
		printf("cache percent\t%16.6f\n\n", 100.0*(Flt)nShadowCacheHits/(Flt)nShadows);
	} else {
		printf("cache percent\n\n");
	}

	printf("avg rays/pixel\t%16.6f\n", (Flt)nRays / ((Flt)(line+1-start_line)*(Flt)Xresolution));
	printf("avg queues/ray\t%16.6f\n\n", (Flt)totalQueues / (Flt) totalQueueResets);

	printf("bounds checked\t\t%8lu\n", nChecked);
	printf("queue inserts\t\t%8lu\n", totalQueues);
	printf("queue resets\t\t%8lu\n", totalQueueResets);
	printf("max queue size\t\t%8lu\n", maxQueueSize);

	printf("\nmax recursion depth       %3d/%d  ", deepest+1, maxlevel);

	fflush(stdout);

}       /* end of statistics() */

