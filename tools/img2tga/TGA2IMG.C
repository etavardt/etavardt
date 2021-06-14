/*
	tga2img -- converts 24 bit uncompressed Targa files to .img format
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

int             xres, yres;             /* image size */
unsigned char   *red, *grn, *blu;       /* tmp storage for 1 scan line */
FILE            *ifp, *ofp;             /* input and output file ptrs */

main(int ac, char **av)
{
	char    infile[256], outfile[256];
	int     i, y;

	if(ac != 2) {
		printf("Usage: tga2img <file_root>\n");
		exit(0);
	}

	strcpy(infile, av[1]);
	strcat(infile, ".tga");
	ifp = fopen(infile, READ_MODE);
	if(!ifp) {
		fprintf(stderr, "Error opening file %s for input.\n", infile);
		exit(1);
	}

	strcpy(outfile, av[1]);
	strcat(outfile, ".img");
	ofp = fopen(outfile, WRITE_MODE);
	if(!ofp) {
		fprintf(stderr, "Error opening file %s for output.\n", outfile);
		exit(1);
	}

	/*
		Read targa header. 3rd byte should be 2 signifying a
		type 2 targa file.  Get resolution.
	*/

	fgetc(ifp);
	fgetc(ifp);
	if(fgetc(ifp) != 2) {
		fprintf(stderr, "Sorry, but this program only works for type 2 targa files.\n");
		exit(1);
	}
	for(i=3; i<12; i++) {
		fgetc(ifp);
	}
	/* get res */
	xres = fgetc(ifp);
	xres += fgetc(ifp)<<8;
	yres = fgetc(ifp);
	yres += fgetc(ifp)<<8;
	/* get last two bytes of the header */
	fgetc(ifp);
	fgetc(ifp);
	printf("image size : %d x %d\n", xres, yres);

	/* allocate temp array space */

	red = (unsigned char *)malloc(xres * sizeof(unsigned char));
	if(!red) {
		fprintf(stderr, "Error allocating space for red array.\n");
		exit(1);
	}
	grn = (unsigned char *)malloc(xres * sizeof(unsigned char));
	if(!grn) {
		fprintf(stderr, "Error allocating space for green array.\n");
		exit(1);
	}
	blu = (unsigned char *)malloc(xres * sizeof(unsigned char));
	if(!blu) {
		fprintf(stderr, "Error allocating space for blue array.\n");
		exit(1);
	}

	/* write .img file header */
	fputc(xres/256, ofp);
	fputc(xres&0xff, ofp);
	fputc(yres/256, ofp);
	fputc(yres&0xff, ofp);
	fputc(0, ofp);
	fputc(0, ofp);
	fputc(yres/256, ofp);
	fputc(yres&0xff, ofp);
	fputc(0, ofp);
	fputc(24, ofp);

	for(y=0; y<yres; y++) {
		/* let user know we're awake */
		if(y%10 == 0) {
			printf("%c%4d", 13, y);
		}
		/* fill array from targa file */
		for(i=0; i<xres; i++) {
			red[i] = fgetc(ifp);
			grn[i] = fgetc(ifp);
			blu[i] = fgetc(ifp);
		}
		/* write out to .img file */
		img_write();
	}

	/* wave goodbye */

	fclose(ifp);
	fclose(ofp);

	exit(0);

}       /* end of main() */

/*
	img_write() -- output a scan line of a .img file.
*/

img_write()
{
	int     i,              /* which pixel? */
		total,          /* how many left in scan? */
		count,          /* current run total */
		or, og, ob,     /* current run color */
		r, g, b;        /* next pixel color */

	i = 0;
	total = xres;
	or = red[i];
	og = grn[i];
	ob = blu[i];
	i++;
	do {
		count = 1;
		total--;
		while(1) {
			r = red[i];
			g = grn[i];
			b = blu[i];
			i++;
			if(r!=or || g!=og || b!=ob || count>=254 || total<=0) {
				break;
			}
			total--;
			count++;
		}
		if(fputc(count, ofp) == EOF) {
			fprintf(stderr, "Error writing to disk.  Must be out of space.\n");
			exit(1);
		}
		fputc(or, ofp);
		fputc(og, ofp);
		fputc(ob, ofp);

		or = r;
		og = g;
		ob = b;

		if(total==1) {          /* if at last pixel */
			fputc(1, ofp);
			fputc(red[xres-1], ofp);
			fputc(grn[xres-1], ofp);
			fputc(blu[xres-1], ofp);
			total--;
		}
	} while(total>0);

}       /* end of img_write() */
