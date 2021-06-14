/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                               Hex Generator                             บ
บ                                                                         บ
บ      Hexer.C = generates a surface of hexagontal patterns of spheres    บ
บ                                                                         บ
บ       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     บ
บ                                                                         บ
บ       ALL RIGHTS RESERVED.   This software is published, but is NOT     บ
บ         Public Domain and remains the propery of ALGORITHM, Inc.,       บ
บ   Christopher D. Watkins and Stephen B. Coy.  This software may not be  บ
บ  reproduced or integrated into other packages without the prior written บ
บ          consent of Christopher D. Watkins and Stephen B. Coy.          บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_EDGE        (16)

main()
{
	int             i, j;
	double          sqrt3, x, y;

	sqrt3 = sqrt(3.0);

	for(i=0; i<NUM_EDGE; i++) {
		y = sqrt3 * i;
		for(j=0; j<NUM_EDGE+NUM_EDGE-i-1; j++) {
			x = j*2 - (NUM_EDGE-1)*2 + i;
			printf("surf { diff %.3f %.3f %.3f shine 1000 1 1 1 }\n", (double)rand()/RAND_MAX, (double)rand()/RAND_MAX, (double)rand()/RAND_MAX);
			printf("sphere { center %.4f %.4f 0 radius 1 }\n", x, y);
			if(i != 0) {
				printf("surf { diff %.3f %.3f %.3f shine 1000 1 1 1 }\n", (double)rand()/RAND_MAX, (double)rand()/RAND_MAX, (double)rand()/RAND_MAX);
				printf("sphere { center %.4f %.4f 0 radius 1 }\n", x, -y);
			}
		}
		x -= 1.0;
	}
}
