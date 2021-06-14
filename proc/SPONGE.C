/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                               Menger Sponge                             บ
บ                                                                         บ
บ                Sponge.C = recursive Menger Sponge generator             บ
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

#define MAX_LEVEL       (3)

int     max_level = MAX_LEVEL;

main(int ac, char **av)
{
	int     foo(double, double, double, double, int);

	if(ac == 2)
		max_level = atoi(av[1]);

	foo(0.0, 0.0, 0.0, 1.0, 0);
}

foo(x, y, z, rad, level)
	double  x, y, z, rad;
	int     level;
{
	double  offset;
	int     i, j, k;

	/* spit out current level */
	printf("sphere { center %.4f %.4f %.4f radius %.4f }\n", x, y, z, rad);

	level++;
	if(level > max_level)
		return;

	offset = rad * 2.0;
	rad /= 3.0;
	for(i=-1; i<2; i++) {
		for(j=-1; j<2; j++) {
			for(k=-1; k<2; k++) {
				if(i || j || k) {
					foo(x+i*offset, y+j*offset, z+k*offset, rad, level);
				}
			}
		}
	}
}
