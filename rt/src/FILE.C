/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                             Bob Ray Tracer                              บ
บ                                                                         บ
บ        File.C = functions to handle opening of files taking into        บ
บ		   consideration environment variables.			  บ
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
#include <string.h>

#define BOB_ENV         "BOB"
#define DELIMS          ";"
#define MAX_PATHS       (32)

static int      num_paths;
static char     *paths[MAX_PATHS];

/*
	init_env() -- reads the environment and sets up the
		paths[] array.
*/

void    init_env()
{
	char    *path, *sub;

	num_paths = 1;
	paths[0] = "";          /* set first path to nothing */

	sub = getenv(BOB_ENV);
	path = strdup(sub);             /* make a local copy to work with */

	sub = strtok(path, DELIMS);
	while(sub && num_paths < MAX_PATHS) {
		paths[num_paths] = strdup(sub);
		num_paths++;
		sub = strtok(NULL, DELIMS);
	}

}       /* end of init_env() */

/*
	env_fopen() -- acts just like fopen except uses the paths
		taken from the envoronment variable BOB.  Also
		handles filenames wrapped in quotes.
*/

FILE *env_fopen(char *name, char *mode)
{
	FILE            *fp;
	int             i;
	static char     full_path[1024];

	for(i=0; i<num_paths; i++) {
		strcpy(full_path, paths[i]);
		if(i) {
			strcat(full_path, "/");
		}
		qstrcat(full_path, name);
		fp = fopen(full_path, mode);
		if(fp) {
			return fp;
		}
	}

	/* nothing worked so return NULL */
	return NULL;

}       /* end of env_fopen() */

/*
	qstrcat() -- just like strcat() except ignores quotes
*/

qstrcat(char *d, char *s)
{
	/* find end of dest */
	while(*d)
		d++;

	/* tack on source */
	while(*s) {
		if(*s != '"') {
			*d = *s;
			d++;
		}
		s++;
	}
	*d = *s;        /* copy final NULL */
}       /* end of qstrcat() */
