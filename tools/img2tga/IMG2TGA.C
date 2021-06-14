/*
ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
บ                                                                         บ
บ                          IMG to TGA conversion                          บ
บ                                                                         บ
บ              IMG2TGA.C = IMG to TGA 24-bit color conversion             บ
บ       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     บ
บ                                                                         บ
ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ

	img2tga -- converts .img format files to 24 bit
		uncompressed Targa files.
*/

#ifdef MSDOS
	#define READ_MODE       "rb"
	#define WRITE_MODE      "wb"
#else
	#define READ_MODE       "r"
	#define WRITE_MODE      "w"
#endif

#include <stdio.h>
#include <stdlib.h>

main(int ac, char **av)
{
	int     xres, yres;             /* image size */
	FILE    *ifp, *ofp;             /* input and output file ptrs */
	char    infile[256], outfile[256];
	int     i, x, y, total, count, red, grn, blu;

	if(ac != 2) {
		printf("Usage: img2tga <file_root>\n");
		exit(0);
	}

	strcpy(infile, av[1]);
	strcat(infile, ".img");
	ifp = fopen(infile, READ_MODE);
	if(!ifp) {
		fprintf(stderr, "Error opening file %s for input.\n", infile);
		exit(1);
	}

	strcpy(outfile, av[1]);
	strcat(outfile, ".tga");
	ofp = fopen(outfile, WRITE_MODE);
	if(!ofp) {
		fprintf(stderr, "Error opening file %s for output.\n", outfile);
		exit(1);
	}

	/* Read .img header.  Get resolution and toss the rest. */

	xres = fgetc(ifp)<<8;
	xres += fgetc(ifp);
	yres = fgetc(ifp)<<8;
	yres += fgetc(ifp);

	fgetc(ifp);
	fgetc(ifp);
	fgetc(ifp);
	fgetc(ifp);
	fgetc(ifp);
	fgetc(ifp);

	/* write .tga header */

	fputc(0, ofp);
	fputc(0, ofp);
	fputc(2, ofp);          /* type 2 targa file */
	for(i=3; i<12; i++) {
		fputc(0, ofp);
	}
	fputc(xres&0xff, ofp);
	fputc(xres/256, ofp);
	fputc(yres&0xff, ofp);
	fputc(yres/256, ofp);
	fputc(24, ofp);         /* bits per pixel */
	fputc(32, ofp);         /* image descriptor */

	printf("image size : %d x %d\n", xres, yres);

	for(y=0; y<yres; y++) {         /* for each scan line */
		/* let user know we're awake */
		if(y%10 == 0) {
			printf("%c%4d", 13, y);
		}
		total = xres;
		while(total>0) {
			count = fgetc(ifp);
			total -= count;
			red = fgetc(ifp);
			grn = fgetc(ifp);
			blu = fgetc(ifp);
			for(i=0; i<count; i++) {
				fputc(red, ofp);
				fputc(grn, ofp);
				fputc(blu, ofp);
			}
		}
	}

	/* wave goodbye */

	fclose(ifp);
	fclose(ofp);

	exit(0);

}       /* end of main() */
