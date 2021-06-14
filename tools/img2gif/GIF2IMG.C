/*
	gif2img -- convert a gif file to an img file.

	Uses decoder.c for lzw decoding.
*/

#include <stdio.h>

#define MAXX            (2048)

#define LOCAL_MAP       (0x80)
#define INTERLACE       (0x40)

FILE    *infp, *outfp;
char    infile[128], outfile[128];
int     width, height;
int     bad_code_count = 0;             /* global error count for decoder */
int     pal[256][3];

main(int ac, char **av)
{
	int     i, c, colors;
	char    sig[6];         /* GIF signature */

	if(ac != 2) {
		fprintf(stderr, "Usage:  %s <file>\n", av[0]);
		exit(1);
	}

	strcpy(infile, av[1]);
	strcat(infile, ".gif");
	infp = fopen(infile, "rb");
	if(!infp) {
		fprintf(stderr, "Error opening file %s for input.\n", infile);
		last_call(1);
	}
	strcpy(outfile, av[1]);
	strcat(outfile, ".img");
	outfp = fopen(outfile, "wb");
	if(!outfp) {
		fprintf(stderr, "Error opening file %s for output.\n", outfile);
		last_call(1);
	}

	/* check gif signature */

	c = fread(sig, 1, 6, infp);
	if(c!=6 || strncmp(sig, "GIF87a", 6)) {
		fprintf(stderr, "Error in GIF signature.\n");
		last_call(1);
	}

	/* get screen descriptor info */

	width = fgetc(infp);
	width += 256*fgetc(infp);
	height = fgetc(infp);
	height += 256*fgetc(infp);

	/* output width, height, start, stop, and depth */

	printf("Image size = %dx%d\n", width, height);

	putc(width/256, outfp);
	putc(width%256, outfp);
	putc(height/256, outfp);
	putc(height%256, outfp);
	putc(0, outfp); putc(0, outfp);
	putc(height/256, outfp); putc(height%256, outfp);
	putc(0, outfp); putc(24, outfp);

	/* check color map info */

	c = fgetc(infp);
	if((c&LOCAL_MAP) == 0) {
		fprintf(stderr, "Oops, I can only handle images with global color maps.\n");
		last_call(1);
	}
	colors = 1<<((c&0x07)+1);
	fgetc(infp);
	fgetc(infp);

	/* get global color map */

	for(i=0; i<colors; i++) {
		pal[i][0] = fgetc(infp);
		pal[i][1] = fgetc(infp);
		pal[i][2] = fgetc(infp);
	}

	/* get image separator */

	c = fgetc(infp);
	if(c != ',') {
		fprintf(stderr, "Oops, image separator character expected.\n");
		last_call(1);
	}

	/* waste left top width and height */

	for(i=0; i<8; i++)
		fgetc(infp);

	/* check for interlace or local color map */

	c = fgetc(infp);
	if(c & LOCAL_MAP) {
		fprintf(stderr, "Oops, can't handle local color maps.\n");
		last_call(1);
	}
	if(c & INTERLACE) {
		fprintf(stderr, "Oops, can't handle interlace. Try converting with Vpic.\n");
		last_call(1);
	}

	/* whoa, hey, we're ready to decode this hummer */

	printf("GIF decoding in progress.\n\tline     ");
	c = decoder(width);

	fclose(outfp);
	fclose(infp);

	if(c<0) {
		fprintf(stderr, "Decoder returned %d.\n", c);
	}
	if(bad_code_count) {
		fprintf(stderr, "Bad code count, possible corruption.\n");
	}
	fprintf(stderr, "\nThat's all folks!\n");

	exit(0);

}       /* end of main() */

/*
	get_byte() -- return next byte of input file, used by decoder
*/

int     get_byte(void)
{
	return fgetc(infp);
}

/*
	out_line() -- output next scan line, called by decoder
*/

int     out_line(unsigned char pix[], int linelen)
{
	int     buf[MAXX][3];   /* scan line buffer */
	int     i,              /* which pixel? */
		total,          /* how many left in scan? */
		count,          /* current run total */
		or, og, ob,     /* current run color */
		r, g, b;        /* next pixel color */
	static int      line = 0;

	/* print out status line */

	printf("\b\b\b\b\b%5d", line++);

	/* check for odd line size */

	if(linelen != width) {
		fprintf(stderr, "Warning.  GIF decoder returned line of length %d.\n", linelen);
	}

	/* convert pix entries to rgb buf values */

	for(i=0; i<linelen; i++) {
		buf[i][2] = pal[pix[i]][0];
		buf[i][1] = pal[pix[i]][1];
		buf[i][0] = pal[pix[i]][2];
	}

	/* output encoded scan line */

	i = 0;
	total = width;
	or = buf[i][2];
	og = buf[i][1];
	ob = buf[i][0];
	i++;
	do {
		count = 1;
		total--;
		for(;;) {
			r = buf[i][2];
			g = buf[i][1];
			b = buf[i][0];
			i++;
			if(r!=or || g!=og || b!=ob || count>=255 || total<=0) {
				break;
			}
			total--;
			count++;
		}
		fputc(count, outfp);
		fputc(ob, outfp);
		fputc(og, outfp);
		fputc(or, outfp);

		or = r;
		og = g;
		ob = b;

		if(total==1) {          /* if at last pixel */
			fputc(1, outfp);
			fputc(buf[width-1][0], outfp);
			fputc(buf[width-1][1], outfp);
			fputc(buf[width-1][2], outfp);
			total--;
		}
	} while(total>0);

	return 0;
}

/*
	last_call() -- time to bail out
*/

last_call(int exit_val)
{
	if(infp)
		fclose(infp);
	if(outfp)
		fclose(outfp);
	exit(exit_val);
}       /* end of last_call() */
