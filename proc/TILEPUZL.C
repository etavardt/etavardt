/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                           Tile Puzzle Generator                         บ
บ                                                                         บ
บ                 TilePuzl.C = generates rounded-edged tiles              บ
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

#define RADIUS  (0.1)
#define EDGE    (10)
#define TWEEK   (0.3)

double  grid[EDGE][EDGE][2];

main()
{
	int     i, j, k;
	double  rnd();

	/* init grid */
	for(i=0; i<EDGE; i++) {
		for(j=0; j<EDGE; j++) {
			grid[i][j][0] = i;
			grid[i][j][1] = j;
		}
	}

	/* tweek inner grid points if needed */
	if(TWEEK > 0.0) {
		for(i=1; i<EDGE-1; i++) {
			for(j=1; j<EDGE-1; j++) {
				if(rand() & 0x0100)
					grid[i][j][0] += rnd()*TWEEK;
				else
					grid[i][j][0] -= rnd()*TWEEK;
				if(rand() & 0x0400)
					grid[i][j][1] += rnd()*TWEEK;
				else
					grid[i][j][1] -= rnd()*TWEEK;
			}
		}
	}

	/* output tiles */
	for(i=0; i<EDGE-1; i++) {
		for(j=0; j<EDGE-1; j++) {
			/* top surface */
			printf("polygon { points 4\n");
			printf("\tvertex %.3f %.3f %.3f\n", grid[i][j][0]+RADIUS, grid[i][j][1]+RADIUS, RADIUS);
			printf("\tvertex %.3f %.3f %.3f\n", grid[i][j+1][0]+RADIUS, grid[i][j+1][1]-RADIUS, RADIUS);
			printf("\tvertex %.3f %.3f %.3f\n", grid[i+1][j+1][0]-RADIUS, grid[i+1][j+1][1]-RADIUS, RADIUS);
			printf("\tvertex %.3f %.3f %.3f\n", grid[i+1][j][0]-RADIUS, grid[i+1][j][1]+RADIUS, RADIUS);
			printf("}\n");
			/* spheres */
			printf("sphere { center %.3f %.3f 0 radius %.3f }\n", grid[i][j][0]+RADIUS, grid[i][j][1]+RADIUS, RADIUS);
			printf("sphere { center %.3f %.3f 0 radius %.3f }\n", grid[i][j+1][0]+RADIUS, grid[i][j+1][1]-RADIUS, RADIUS);
			printf("sphere { center %.3f %.3f 0 radius %.3f }\n", grid[i+1][j+1][0]-RADIUS, grid[i+1][j+1][1]-RADIUS, RADIUS);
			printf("sphere { center %.3f %.3f 0 radius %.3f }\n", grid[i+1][j][0]-RADIUS, grid[i+1][j][1]+RADIUS, RADIUS);
			/* cylinders */
			printf("cone { apex %.3f %.3f 0 base %.3f %.3f 0 radius %.3f }\n", grid[i][j][0]+RADIUS, grid[i][j][1]+RADIUS, grid[i+1][j][0]-RADIUS, grid[i+1][j][1]+RADIUS, RADIUS);
			printf("cone { apex %.3f %.3f 0 base %.3f %.3f 0 radius %.3f }\n", grid[i][j][0]+RADIUS, grid[i][j][1]+RADIUS, grid[i][j+1][0]+RADIUS, grid[i][j+1][1]-RADIUS, RADIUS);
			printf("cone { apex %.3f %.3f 0 base %.3f %.3f 0 radius %.3f }\n", grid[i+1][j+1][0]-RADIUS, grid[i+1][j+1][1]-RADIUS, grid[i+1][j][0]-RADIUS, grid[i+1][j][1]+RADIUS, RADIUS);
			printf("cone { apex %.3f %.3f 0 base %.3f %.3f 0 radius %.3f }\n", grid[i+1][j+1][0]-RADIUS, grid[i+1][j+1][1]-RADIUS, grid[i][j+1][0]+RADIUS, grid[i][j+1][1]-RADIUS, RADIUS);
		}
	}

}       /* end of main() */

double  rnd()
{
	return (double)rand()/RAND_MAX;
}
