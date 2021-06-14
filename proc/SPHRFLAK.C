/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                                Sphere Flake                             บ
บ                                                                         บ
บ             SphrFlak.C = generates a sphereflake from spheres           บ
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

	flake -- Create a sphereflake-like object.  Inspired by
		Eric Haines' work.

	Note: This is kind of sloppy since it generates spheres
		which are internal to other spheres.  Don't worry
		about it too much though.  Also be aware that the
		number of spheres this generates goes up pretty
		fast as you increase the recursion level.

		# spheres = sum of i=0,level-1 of 12**i

		level   # spheres
		  1           1     12**0
		  2          13     12**0 + 12**1
		  3         157     12**0 + 12**1 + 12**2
		  4        1885     12**0 + 12**1 + 12**2 + 12**3
		  5       22621     12**0 + 12**1 + 12**2 + 12**3 + 12**4
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX             (3)
#define RADIUS          (1.0)
#define RADIUS_SCALE    (3.0)

main(int ac, char **av)
{
	int     max_level = MAX;

	if(ac == 2) {
		max_level = atoi(av[1]);
	}

	flake(RADIUS, max_level);
}       /* end of main() */

flake(double radius, int level)
{
	double  new_radius;
	int     i;

	new_radius = radius / RADIUS_SCALE;

	/* output current sphere */
	printf("sphere { center 0 0 0 radius %g }\n", radius);

	if(level <= 1) {
		return;
	}

	--level;

	/* output 12 sub-spheres */

	/* do 6 around equator first */
	for(i=0; i<6; i++) {
		printf("transform { translate 0 %g 0 rotate 0 0 %d }\n",
			radius+new_radius, i*60);
		flake(new_radius, level);
		printf("transform_pop\n");
	}

	/* now top 3 */
	for(i=0; i<3; i++) {
		printf("transform { translate 0 %g 0 rotate 54.7356 0 %d }\n",
			radius+new_radius, 30 + i*120);
		flake(new_radius, level);
		printf("transform_pop\n");
	}

	/* now bottom 3 */
	for(i=0; i<3; i++) {
		printf("transform { translate 0 %g 0 rotate -54.7356 0 %d }\n",
			radius+new_radius, 90 + i*120);
		flake(new_radius, level);
		printf("transform_pop\n");
	}

}       /* end of flake */
