/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                        Recursive 3-D Tree Generator                     บ
บ                                                                         บ
บ           Trees.C = recursive trees from spheres and cylinders          บ
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

	trees -- Generates strange looking recursive trees.  Unless
		you have lots of disk space and time you shouldn't
		go much beyond level 8 or so.

	NUM_BRANCHES -- Controls how many new branches are created
		at each joint.
	START_RADIUS -- The starting trunk radius.
	RADIUS_SCALE -- Scale factor by which the branch radius is
		reduced at each level.
	START_LENGTH -- Starting branch segment length.
	LENGTH_SCALE -- Scale factor by which the branch segment length
		is adjusted at each level.  Does not need to be less
		than one.
	MAX_ANGLE -- The maximum angle a branch will bend from its parent.
	LEAF_RADIUS -- Size of the leaves.  Way ugly things, too.

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DEFAULT_MAX_LEVEL       (5)
#define NUM_BRANCHES            (2)
#define START_RADIUS            (6.0)
#define RADIUS_SCALE            (0.6)
#define START_LENGTH            (30.0)
#define LENGTH_SCALE            (0.9)
#define MAX_ANGLE               (60)
#define LEAF_RADIUS             (3.0)

#define LEAF_SURF       (0)
#define BRANCH_SURF     (1)

int     start_level = DEFAULT_MAX_LEVEL;

main(int ac, char **av)
{
	if(ac == 2) {
		start_level = atoi(av[1]);
	}

	/* output sphere at base */
	surface(BRANCH_SURF);
	printf("sphere { center 0 0 0 radius %g }\n", START_RADIUS);

	/* build a tree */
	tree(START_LENGTH, START_RADIUS, start_level);

}       /* end of main() */

tree(double length, double radius, int level)
{
	int     branch;
	double  new_length, new_radius, new_level;

	if(level == 0) {
		surface(LEAF_SURF);
		printf("ring { center 0 0 0 normal %d %d %d radius %g }\n",
			rnd(360), rnd(360), rnd(360), LEAF_RADIUS);
		return;
	}

	new_length = length * LENGTH_SCALE;
	new_radius = radius * RADIUS_SCALE;
	new_level = level - 1;

	surface(BRANCH_SURF);

	/* rotate current branch */
	if(level != start_level) {
		printf("transform { rotate %d %d %d }\n",
			rnd(MAX_ANGLE)-rnd(MAX_ANGLE),
			rnd(MAX_ANGLE)-rnd(MAX_ANGLE),
			rnd(MAX_ANGLE)-rnd(MAX_ANGLE));
	}

	/* output current segment */
	printf("cone { base 0 0 0 apex 0 0 %g base_radius %g apex_radius %g }\n",
		length, radius, new_radius);
	printf("sphere { center 0 0 %g radius %g }\n", length, new_radius);

	/* move coordinate system to tip */
	printf("transform { translate 0 0 %g }\n", length);

	/* build branches */
	for(branch=0; branch<NUM_BRANCHES; branch++) {
		tree(new_length, new_radius, new_level);
	}

	/* pop coordinate sysem move */
	printf("transform_pop\n");

	/* pop rotation */
	if(level != start_level) {
		printf("transform_pop\n");
	}

}       /* end of tree */

int     rnd(int i)
{
	return (int)(((double)rand()/(double)RAND_MAX) * i);
}

surface(int surf)
{
	static int      prev_surf = (-1);

	if(surf == prev_surf) {
		return;
	}

	if(surf == LEAF_SURF) {
		printf("surface { diffuse green }\n");
	} else {
		printf("surface { diffuse medium_tan }\n");
	}

        prev_surf = surf;

}       /* end of surface() */
