/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                              Bob Ray Tracer                             บ
บ                                                                         บ
บ                        Main.C = in the beginning...                     บ
บ                                                                         บ
บ       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     บ
บ                                                                         บ
บ       ALL RIGHTS RESERVED.   This software is published, but is NOT     บ
บ         Public Domain and remains the propery of ALGORITHM, Inc.,       บ
บ   Christopher D. Watkins and Stephen B. Coy.  This software may not be  บ
บ  reproduced or integrated into other packages without the prior written บ
บ          consent of Christopher D. Watkins and Stephen B. Coy.          บ
บ                                                                         บ
บ                     Requires : defs.h, extern.h                         บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "extern.h"

main(ac, av)
	int	ac;
	char	**av;
{
	int	i;
	int     xres = (-1), yres = (-1), depth = (-1), amode = (-1),
		start = (-1), stop = (-1), bunch = (-1);
	int     preprocess = 1;
	char    infilename[80];
	char	outfilename[80];
	void    usage(char *);

	init_env();     /* init environment paths */

	/* clip path off of program name */
	if((Progname = rindex(av[0], '\\')) == NULL)
		Progname = av[0];
	else 
		Progname++;
	strlwr(Progname);

	/* init global clips before parser is called */
	GlobalClipTop = (GlobalClip *)vmalloc(sizeof(GlobalClip));
	ptrchk(GlobalClipTop, "global clip structure");
	GlobalClipTop->next = NULL;
	GlobalClipTop->clip = NULL;

	infilename[0] = 0;		/* to be safe */

	tickflag = 1;                   /* default to full stats */
	resume = 0;
	for(i=1; i<ac; i++) {		/* loop through command line args */
		if(av[i][0] == '-') {
			switch(av[i][1]) {
				case 'i':       /* set resolution */
					i++;
					xres = atoi(av[i]);
					i++;
					yres = atoi(av[i]);
					break;
				case 'l':       /* set start & stop line */
					i++;
					start = atoi(av[i]);
					i++;
					stop = atoi(av[i]);
					break;
				case 'd':       /* set max recursion depth */
					i++;
					depth = atoi(av[i]);
					break;
				case 'b':       /* set bunching factor */
					i++;
					bunch = atoi(av[i]);
					break;
				case 'a':
					i++;
					switch(av[i][0]) {
						case 'n' : amode = A_NONE; break;
						case 'c' : amode = A_CORNERS; break;
						case 'q' : amode = A_QUICK; break;
						case 'a' : amode = A_ADAPTIVE; break;
						default :
							fprintf(stderr, "Error, unkown antialiasing mode %s.\n", av[i]);
							usage(Progname);
							break;
					}
					break;
				case 'n':
					no_shadows = 1;
					break;
				case 'r':
					resume = 1;
					break;
				case 's':
					tickflag = 0;
					break;
				case 'p':
					preprocess = 0;
					break;
				case '?':
					usage(Progname);	/* no return */
					break;
				default:
					fprintf(stderr, "Error, unkown command line flag %s.\n", av[i]);
					usage(Progname);
					break;
			}	/* end of switch */
		} else {
			strcpy(infilename, av[i]);
		}
	}

	if(infilename[0] == 0)		/* no file name given */
		usage(Progname);	/* no return */
		
	strcpy(outfilename, infilename);
	strcat(infilename, ".b");
	strcat(outfilename, ".img");

	camera.aperture = -1.0;
	camera.focal_length = 0.0;
	Eye.view_aspect = -1.0;

	if(tickflag) {
		fprintf(stderr, "%s    %s\n%s\n", _Program, _Version, _Copyright);
	}

	stop_line = (-1);       /* helps to catch no studio error */
	if(preprocess) {
		preproc(infilename, "yyz.b");
		ReadSceneFile(infilename, "yyz.b");
		/* remove("yyz.b"); */
	} else {
		ReadSceneFile(infilename, infilename);
	}

	if(Eye.view_aspect != -1.0) {
		Eye.view_angle_y = Eye.view_angle_x / Eye.view_aspect;
	}
	if(xres > 0) {          /* if command line override */
		Xresolution = xres;
		if(stop_line == Yresolution)    /* only change if not default */
			stop_line = yres;
		Yresolution = yres;
	}
	if(start != -1) {       /* if command line override */
		start_line = start;
		stop_line = stop;
	}
	if(depth > 0) {         /* if command line override */
		maxlevel = depth;
	}
	if(bunch > 0) {         /* if command line override */
		bunching = bunch;
	}
	if(amode >= 0) {        /* if command line override */
		antialias = amode;
	}

	BuildBoundingSlabs();
	init_noise();

	Screen(&Eye, outfilename, Xresolution, Yresolution);

	last_call(0);

}       /* end of main() */

void    usage(Progname)
	char    *Progname;
{
	fprintf(stderr, "%s        %s        %s\n%s\n\n", _Program, _Version, _Date, _Copyright);
	fprintf(stderr, "Usage:  %s [flags] <file>\n", Progname);
	fprintf(stderr, "       -s set silent mode\n");
	fprintf(stderr, "       -r resume generating an interrupted image.\n");
	fprintf(stderr, "       -i change image size to xres by yres.\n");
	fprintf(stderr, "       -l change start and stop line.\n");
	fprintf(stderr, "       -n no shadows\n");
	fprintf(stderr, "       -d limit recursion level to depth.\n");
	fprintf(stderr, "       -a change antialiasing to mode.  Valid modes are:\n");
	fprintf(stderr, "          none, corners, quick, and adaptive.\n");
	fprintf(stderr, "       -p run without the preprocessor.\n");
	fprintf(stderr, "       -b set bunching factor.\n");
	fprintf(stderr, "Assumes file.b for input file and file.img for output file.\n");

	last_call(0);
}

last_call(int return_code)
{
	exit(return_code);
}
