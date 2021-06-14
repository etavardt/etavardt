/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                             Torus Generator                             บ
บ                                                                         บ
บ         Torus.C = create a torus from spheres, patches or facets        บ
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

#define PI      (3.1415927)

#define SPHERES (0)
#define SMOOTH  (1)
#define FACETS  (2)

main(int ac, char **av)
{
	int     mode;
	int     seg1, seg2, i, j;
	double  lr, sr, t0, t1, p0, p1, dt, dp;
	double  st0, st1, ct0, ct1, sp0, sp1, cp0, cp1;
	double  x0, x1, y0, y1;

	if(ac != 6) {
		usage(av[0]);
	}

	lr = atof(av[1]);
	sr = atof(av[2]);
	seg1 = atoi(av[3]);
	seg2 = atoi(av[4]);
	if(strcmp(av[5], "spheres") == 0) {
		mode = SPHERES;
	} else if(strcmp(av[5], "facets") == 0) {
		mode = FACETS;
	} else if(strcmp(av[5], "patches") == 0) {
		mode = SMOOTH;
	} else {
		fprintf(stderr, "Error, unkown mode type %s\n", av[5]);
		fprintf(stderr, "Mode must be either spheres, patches or facets.\n");
		exit(1);
	}

	dt = 2.0 * PI / seg1;
	dp = 2.0 * PI / seg2;
	for(i=0; i<seg1; i++) {
		t0 = i * dt;
		t1 = ((i+1)%seg1) * dt;
		st0 = sin(t0); ct0 = cos(t0);
		st1 = sin(t1); ct1 = cos(t1);

		if(mode == SPHERES) {
			printf("sphere { center %g %g %g radius %g }\n", st0*lr, ct0*lr, 0.0, sr);
			printf("cone { apex %g %g %g base %g %g %g radius %g }\n", st0*lr, ct0*lr, 0.0, st1*lr, ct1*lr, 0.0, sr);
		} else {
			for(j=0; j<seg2; j++) {
				p0 = j * dp;
				p1 = ((j+1)%seg2) * dp;
				sp0 = sin(p0); cp0 = cos(p0);
				sp1 = sin(p1); cp1 = cos(p1);
				x0 = lr * ct0;
				y0 = lr * st0;
				x1 = lr * ct1;
				y1 = lr * st1;

				if(mode == SMOOTH) {
					printf("patch {\n");
					printf("\tvertex %g %g %g normal %g %g %g\n", x0+sr*cp0*ct0, y0+sr*cp0*st0, sr*sp0, x0+sr*cp0*ct0-x0, y0+sr*cp0*st0-y0, sr*sp0);
					printf("\tvertex %g %g %g normal %g %g %g\n", x0+sr*cp1*ct0, y0+sr*cp1*st0, sr*sp1, x0+sr*cp1*ct0-x0, y0+sr*cp1*st0-y0, sr*sp1);
					printf("\tvertex %g %g %g normal %g %g %g\n", x1+sr*cp0*ct1, y1+sr*cp0*st1, sr*sp0, x1+sr*cp0*ct1-x1, y1+sr*cp0*st1-y1, sr*sp0);
					printf("}\n");
					printf("patch {\n");
					printf("\tvertex %g %g %g normal %g %g %g\n", x0+sr*cp1*ct0, y0+sr*cp1*st0, sr*sp1, x0+sr*cp1*ct0-x0, y0+sr*cp1*st0-y0, sr*sp1);
					printf("\tvertex %g %g %g normal %g %g %g\n", x1+sr*cp1*ct1, y1+sr*cp1*st1, sr*sp1, x1+sr*cp1*ct1-x1, y1+sr*cp1*st1-y1, sr*sp1);
					printf("\tvertex %g %g %g normal %g %g %g\n", x1+sr*cp0*ct1, y1+sr*cp0*st1, sr*sp0, x1+sr*cp0*ct1-x1, y1+sr*cp0*st1-y1, sr*sp0);
					printf("}\n");
				} else {
					printf("polygon { points 3\n");
					printf("\tvertex %g %g %g\n", x0+sr*cp0*ct0, y0+sr*cp0*st0, sr*sp0);
					printf("\tvertex %g %g %g\n", x0+sr*cp1*ct0, y0+sr*cp1*st0, sr*sp1);
					printf("\tvertex %g %g %g\n", x1+sr*cp0*ct1, y1+sr*cp0*st1, sr*sp0);
					printf("}\n");
					printf("polygon { points 3\n");
					printf("\tvertex %g %g %g\n", x0+sr*cp1*ct0, y0+sr*cp1*st0, sr*sp1);
					printf("\tvertex %g %g %g\n", x1+sr*cp1*ct1, y1+sr*cp1*st1, sr*sp1);
					printf("\tvertex %g %g %g\n", x1+sr*cp0*ct1, y1+sr*cp0*st1, sr*sp0);
					printf("}\n");
				}
			}       /* end j loop around minor radius */
		}       /* end if mode spheres */
	}       /* end i loop around outside */
}       /* end of main() */

usage(char *prog)
{
	fprintf(stderr, "Usage:  %s major_radius minor_radius segs1 segs2 mode\n", prog);
	fprintf(stderr, "        where segs1 is # of segments around major radius\n");
	fprintf(stderr, "        and segs2 is # of segments around minor radius.\n");
	fprintf(stderr, "        (of course segs2 is ignored when mode == spheres\n");
	fprintf(stderr, "        mode can be spheres, patches or facets.\n");

	exit(1);
}       /* end of usage() */
