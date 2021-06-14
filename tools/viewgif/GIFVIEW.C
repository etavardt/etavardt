/*
	showgif -- Displays a gif image.  Note that this only understands
		the older GIF87a standard with global color maps.  Supports
		1024x768, 800x600, 640x480 and 320x200 and chooses the
		mode to use based on the width of the image.

	Uses decoder.c for lzw decoding.
*/

#include <stdlib.h>
#include <stdio.h>
#include <graph.h>

#define MAXX            (2048)

#define LOCAL_MAP       (0x80)
#define INTERLACE       (0x40)

FILE    *infp;
char    infile[128];
int     width, height;
int     bad_code_count = 0;             /* global error count for decoder */
int     pal[256][3];
int     screen_width, screen_height;
long    lpal[256];

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

	/* check color map info */

	c = fgetc(infp);
	if((c&LOCAL_MAP) == 0) {
		fprintf(stderr, "Oops, I can only handle images with global color maps.\n");
		last_call(1);
	}
	colors = 1<<((c&0x07)+1);
	fgetc(infp);
	fgetc(infp);

	/* set graphics mode based on width */

	if(width>800 && _setvideomode(_XRES256COLOR)) {
		screen_width = 1024;
		screen_height = 768;
	} else if(width>640 && _setvideomode(_SVRES256COLOR)) {
		screen_width =  800;
		screen_height = 600;
	} else if(width>320 && _setvideomode(_VRES256COLOR)) {
		screen_width =  640;
		screen_height = 480;
	} else if(_setvideomode(_MRES256COLOR)) {
		screen_width =  320;
		screen_height = 200;
	} else {
		fprintf(stderr, "Error trying to set video mode.\n");
		last_call(1);
	}

	/* get global color map */

	for(i=0; i<colors; i++) {
		long    r, g, b;
		r = pal[i][0] = fgetc(infp);
		g = pal[i][1] = fgetc(infp);
		b = pal[i][2] = fgetc(infp);
		r >>= 2;
		g >>= 2;
		b >>= 2;
		lpal[i] = (b<<16) + (g<<8) + r;
	}

	/* set palette */

	_remapallpalette(lpal);

	/* get image separator */

	c = fgetc(infp);
	if(c != ',') {
		_setvideomode(_DEFAULTMODE);
		fprintf(stderr, "Oops, image separator character expected.\n");
		last_call(1);
	}

	/* waste left top width and height */

	for(i=0; i<8; i++)
		fgetc(infp);

	/* check for interlace or local color map */

	c = fgetc(infp);
	if(c & LOCAL_MAP) {
		_setvideomode(_DEFAULTMODE);
		fprintf(stderr, "Oops, can't handle local color maps.\n");
		last_call(1);
	}
	if(c & INTERLACE) {
		_setvideomode(_DEFAULTMODE);
		fprintf(stderr, "Oops, can't handle interlace. Try converting with Vpic.\n");
		last_call(1);
	}

	/* whoa, hey, we're ready to decode this hummer */

	c = decoder(width);

	fclose(infp);

	/* wait for key hit to exit */

	while(!kbhit())
		;
	getch();        /* get char from buffer */
	_setvideomode(_DEFAULTMODE);

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
	int             i;
	static int      j = 0;

	if(j>=screen_height)
		return 0;

	for(i=0; i<width && i<screen_width; i++) {
		_setcolor(pix[i]);
		_setpixel(i, j);
	}
	j++;

	return 0;
}

/*
	last_call() -- time to bail out
*/

last_call(int exit_val)
{
	if(infp)
		fclose(infp);
	exit(exit_val);
}       /* end of last_call() */
