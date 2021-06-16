/*
�������������������������������������������
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�        File.C = functions to handle opening of files taking into        �
�		   consideration environment variables.                           �
�                                                                         �
�       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     �
�                                                                         �
�       ALL RIGHTS RESERVED.   This software is published, but is NOT     �
�         Public Domain and remains the propery of ALGORITHM, Inc.,       �
�   Christopher D. Watkins and Stephen B. Coy.  This software may not be  �
�  reproduced or integrated into other packages without the prior written �
�          consent of Christopher D. Watkins and Stephen B. Coy.          �
�                                                                         �
�������������������������������������������
*/

#include <cstdio>
#include <cstdlib>
#include <cstring>

#define BOB_ENV         "BOB"
#define DELIMS          ";"
#define MAX_PATHS       (32)

static int      num_paths;
static const char     *paths[MAX_PATHS];

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
	qstrcat() -- just like strcat() except ignores quotes
*/

void qstrcat(char *d, char *s)
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

/*
	env_fopen() -- acts just like fopen except uses the paths
		taken from the envoronment variable BOB.  Also
		handles filenames wrapped in quotes.
*/

FILE *env_fopen(char *name, const char *mode)
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
