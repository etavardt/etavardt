/*
�������������������������������������������������������������������������ͻ
�                                                                         �
�                             Bob Ray Tracer                              �
�                                                                         �
�                Screen.C = screen scanning and anti-aliasing		  �
�                                                                         �
�       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     �
�                                                                         �
�       ALL RIGHTS RESERVED.   This software is published, but is NOT     �
�         Public Domain and remains the propery of ALGORITHM, Inc.,       �
�   Christopher D. Watkins and Stephen B. Coy.  This software may not be  �
�  reproduced or integrated into other packages without the prior written �
�          consent of Christopher D. Watkins and Stephen B. Coy.          �
�                                                                         �
�                    Requires: defs.h, pic.h, extern.h                    �
�                                                                         �
�������������������������������������������������������������������������ͼ

	screen -- Contains the scan functions which determine what
		antialiasing is done and the function shoot() which
		handle the projection mode, depth of field and
		actually calling the trace function.
*/

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "defs.hpp"
#include "pic.hpp"
#include "extern.hpp"

static	Flt	frustrumwidth,
		frustrumheight;
static  Point   viewvec, leftvec, looking_up, viewpoint;
static	Ray	ray;		/* normal, untweeked ray */
static	int	x_res, y_res;
static	Pic	*pic;

void	Screen(Viewpoint *view, char *picfile, int xres, int yres)
{
	ScrInit(view, xres, yres, picfile);

	switch(antialias) {
		case A_NONE:
			Scan0();
			break;
		case A_CORNERS:
			Scan1();
			break;
		case A_QUICK:
			Scan2();
			break;
		case A_ADAPTIVE:
			Scan3();
			break;
	}
	PicClose(pic);
}

void	ScrInit(Viewpoint	*view, int xres, int yres, char *picfile)
{
	/*
	 * open the picture file...
	 */

	pic = PicOpen(picfile, xres, yres);

	/*
	 * determine the viewing frustrum
	 */

	x_res = xres;
	y_res = yres;

	VecNormalize(view->view_up);
	VecSub(view->view_at, view->view_from, viewvec);
	VecNormalize(viewvec);
	VecCross(view->view_up, viewvec, leftvec);
	VecNormalize(leftvec);
	VecS((-1), leftvec, leftvec);   /* convert to right handed */

	/* make view_up vector perpendicular to veiwvec and leftvec */

	if(camera.projection != P_NO_PARALLAX) {
		VecCross(leftvec, viewvec, view->view_up);
		VecNormalize(view->view_up);
	}
	VecCopy(view->view_up, looking_up);

	/* set up camera stuff */

	if(camera.aperture > 0.0) {	/* calc lens vectors */
		VecCopy(leftvec, camera.lens_i);
		VecCross(viewvec, camera.lens_i, camera.lens_j);
		VecNormalize(camera.lens_j);
		VecS(camera.aperture, camera.lens_i, camera.lens_i);
		VecS(camera.aperture, camera.lens_j, camera.lens_j);
	}

	VecCopy(view->view_from, ray.P);
	VecCopy(view->view_from, viewpoint);
	VecCopy(viewvec, ray.D);

	frustrumwidth  = tan(view->view_angle_x);
	frustrumheight = tan(view->view_angle_y);

}	/* end of ScrInit() */

/*
	Scan0 -- Basic one ray per pixel, right through the center.
		 Jitter value +- 0.5
*/

void	Scan0(void)
{
	Pixel	*buf;
	Color	color;		/* color of current traced ray */
	int     i, j;
	Flt     x, y;

	buf = (Pixel *)malloc(x_res * sizeof(Pixel));

	for(j=start_line; j<stop_line; j++) {
		for(i=0; i<x_res; i++) {
			if(jitter) {
				x = i + rand()/(Flt)RAND_MAX;
				y = j + rand()/(Flt)RAND_MAX;
			} else {
				x = i + 0.5;    /* hit center of pixel */
				y = j + 0.5;
			}

			Shoot(x, y, color);

			buf[i].r = (unsigned char) (255.0 * color[0]);
			buf[i].g = (unsigned char) (255.0 * color[1]);
			buf[i].b = (unsigned char) (255.0 * color[2]);
		}
		PicWriteLine(pic, buf);
		if(tickflag)
			statistics(j);
	}
	if(tickflag)
		fprintf(stderr, "\n");

	free(buf);
}	/* end of Scan0() */

/*
	Scan1 -- Shoot a ray at each corner of the pixel and average
		 the results.
		 Jitter value +- 0.5
*/

void	Scan1(void)
{
	Pixel   *buf, *oldbuf, *curbuf, *tmp;
	int	red, green, blue;
	Color	color;		/* color of current traced ray */
	int     i, j;
	Flt     x, y;

	/*
	 * allocate enough space for an entire row of pixels...
	 * plus one more for the average...
	 */

	oldbuf = NULL;
	curbuf = (Pixel *)malloc ((x_res+1) * sizeof (Pixel));
	buf = (Pixel *)malloc ((x_res+1) * sizeof (Pixel));

	for(j=start_line; j<stop_line; j++) {
		for(i=0; i<x_res+1; i++) {
			if(jitter) {
				x = i + rand()/(Flt)RAND_MAX;
				y = j + rand()/(Flt)RAND_MAX;
			} else {
				x = i + 0.5;    /* hit center of pixel */
				y = j + 0.5;
			}
			Shoot(x, y, color);

			curbuf[i].r = (unsigned char) (255.0 * color[0]);
			curbuf[i].g = (unsigned char) (255.0 * color[1]);
			curbuf[i].b = (unsigned char) (255.0 * color[2]);
		}
		if(oldbuf) {
			/* average the pixels, and write 'em out */
			for(i=0; i<x_res; i++) {
				red = ((int)curbuf[i].r + (int)curbuf[i+1].r
				     + (int)oldbuf[i].r+ (int)oldbuf[i+1].r);
				green = ((int)curbuf[i].g + (int)curbuf[i+1].g
				     + (int)oldbuf[i].g+ (int)oldbuf[i+1].g);
				blue = ((int)curbuf[i].b + (int)curbuf[i+1].b
				     + (int)oldbuf[i].b+ (int)oldbuf[i+1].b);
				buf[i].r = (unsigned char) (red / 4);
				buf[i].g = (unsigned char) (green / 4);
				buf[i].b = (unsigned char) (blue / 4);
			}
			PicWriteLine(pic, buf);
			tmp = oldbuf;
			oldbuf = curbuf;
			curbuf = tmp;
		} else {
			oldbuf = curbuf;
			curbuf = (Pixel *) malloc ((x_res + 1) * sizeof (Pixel));
		}

		if(tickflag)
			statistics(j);
	}
	if(tickflag)
		fprintf(stderr, "\n");

	free(buf);
	free(curbuf);
}	/* end of Scan1() */

/*
	Scan2 -- Undersampling, possible 18 to 1 speedup.
		No jitter option.
*/

void    Scan2(void)
{
	Pixel   *buf[7], *buff;
	int     *flags[7];
	Color   color;          /* color of current traced ray */
	int     x, y, xx, yy, i, j;

	int     comp(unsigned int, unsigned int);
	unsigned int    r, g, b;

	/* fill to next mod 6 scan line */
	if(start_line%6) {
		buff = (Pixel *)malloc(x_res * sizeof(Pixel));
		ptrchk(buff, "pixel buffer");

		/* calc stop line for single fill */
		yy = start_line + 6 - (start_line%6);
		for(j=start_line; j<yy; j++) {
			for(i=0; i<x_res; i++) {
				Shoot((Flt)i+0.5, (Flt)j+0.5, color);
				buff[i].r = (unsigned char) (255.0 * color[0]);
				buff[i].g = (unsigned char) (255.0 * color[1]);
				buff[i].b = (unsigned char) (255.0 * color[2]);
			}
			PicWriteLine(pic, buff);
			if(tickflag)
				statistics(j);
		}
		start_line = yy;
		free(buff);
	}

	/* allocate pixel buffers */

	for(i=0; i<7; i++) {
		buf[i] = (Pixel *)malloc((x_res+5) * sizeof(Pixel));
		ptrchk(buf[i], "sampling buffer");
		flags[i] = (int *)malloc((x_res+5) * sizeof(int));
		ptrchk(flags[i], "sampling flag buffer");
	}

	/* start actual sub-sampling */

	for(i=0; i<x_res+5; i++) {      /* clear bottom row of flags */
		flags[6][i] = 0;
	}
	for(y=start_line; y<stop_line; y+=6) {

		/* copy bottom line to top */
		for(i=0; i<x_res+5; i++) {
			if(flags[0][i] = flags[6][i]) { /* only copy if valid */
				buf[0][i].r = buf[6][i].r;
				buf[0][i].g = buf[6][i].g;
				buf[0][i].b = buf[6][i].b;
			}
			/* clear rest of buf */
			for(j=1; j<7; j++) {
				flags[j][i] = 0;
			}
		}

		/* for(x=0; x<x_res+5; x+=6) { */
		for(x=0; x<x_res; x+=6) {
			/* shoot corners and middle */
			i = x; j = 0;
			if(!flags[j][i]) {
				flags[j][i] = 1;
				Shoot((Flt)i+0.5, (Flt)y+0.5, color);
				buf[j][i].r = (unsigned char)(255.0*color[0]);
				buf[j][i].g = (unsigned char)(255.0*color[1]);
				buf[j][i].b = (unsigned char)(255.0*color[2]);
			}

			i = x+6; j = 0;
			if(!flags[j][i]) {
				flags[j][i] = 1;
				Shoot((Flt)i+0.5, (Flt)y+0.5, color);
				buf[j][i].r = (unsigned char)(255.0*color[0]);
				buf[j][i].g = (unsigned char)(255.0*color[1]);
				buf[j][i].b = (unsigned char)(255.0*color[2]);
			}

			i = x; j = 6;
			if(!flags[j][i]) {
				flags[j][i] = 1;
				Shoot((Flt)i+0.5, (Flt)y+0.5+6, color);
				buf[j][i].r = (unsigned char)(255.0*color[0]);
				buf[j][i].g = (unsigned char)(255.0*color[1]);
				buf[j][i].b = (unsigned char)(255.0*color[2]);
			}

			i = x+6; j = 6;
			if(!flags[j][i]) {
				flags[j][i] = 1;
				Shoot((Flt)i+0.5, (Flt)y+0.5+6, color);
				buf[j][i].r = (unsigned char)(255.0*color[0]);
				buf[j][i].g = (unsigned char)(255.0*color[1]);
				buf[j][i].b = (unsigned char)(255.0*color[2]);
			}

			i = x+3; j = 3;         /* middle ray */
			if(!flags[j][i]) {
				flags[j][i] = 1;
				Shoot((Flt)i+0.5, (Flt)y+0.5+3, color);
				buf[j][i].r = (unsigned char)(255.0*color[0]);
				buf[j][i].g = (unsigned char)(255.0*color[1]);
				buf[j][i].b = (unsigned char)(255.0*color[2]);
			}

			/* the corners are shot, now fill in if needed */

			/* check upper left quad first */
			i = x+3; j = (y%6)+3;   /* middle ray */

			if(comp(buf[j][i].r, buf[j-3][i-3].r) &&
			   comp(buf[j][i].g, buf[j-3][i-3].g) &&
			   comp(buf[j][i].b, buf[j-3][i-3].b)) { /* close enough so fill */
				if(!flags[j-1][i]) {
					buf[j-1][i].r = buf[j][i].r;
					buf[j-1][i].g = buf[j][i].g;
					buf[j-1][i].b = buf[j][i].b;
				}
				if(!flags[j][i-1]) {
					buf[j][i-1].r = buf[j][i].r;
					buf[j][i-1].g = buf[j][i].g;
					buf[j][i-1].b = buf[j][i].b;
				}
				if(!flags[j-1][i-1]) {
					buf[j-1][i-1].r = buf[j][i].r;
					buf[j-1][i-1].g = buf[j][i].g;
					buf[j-1][i-1].b = buf[j][i].b;
				}
				if(!flags[j-2][i-2]) {
					buf[j-2][i-2].r = buf[j-3][i-3].r;
					buf[j-2][i-2].g = buf[j-3][i-3].g;
					buf[j-2][i-2].b = buf[j-3][i-3].b;
				}
				if(!flags[j-3][i-2]) {
					buf[j-3][i-2].r = buf[j-3][i-3].r;
					buf[j-3][i-2].g = buf[j-3][i-3].g;
					buf[j-3][i-2].b = buf[j-3][i-3].b;
				}
				if(!flags[j-2][i-3]) {
					buf[j-2][i-3].r = buf[j-3][i-3].r;
					buf[j-2][i-3].g = buf[j-3][i-3].g;
					buf[j-2][i-3].b = buf[j-3][i-3].b;
				}
				r = ((unsigned int)buf[j-3][i-3].r + (unsigned int)buf[j-3][i+3].r) >> 1;
				g = ((unsigned int)buf[j-3][i-3].g + (unsigned int)buf[j-3][i+3].g) >> 1;
				b = ((unsigned int)buf[j-3][i-3].b + (unsigned int)buf[j-3][i+3].b) >> 1;
				r = buf[j-3][i-3].r;
				g = buf[j-3][i-3].g;
				b = buf[j-3][i-3].b;
				if(!flags[j-3][i-1]) {
					buf[j-3][i-1].r = r; buf[j-3][i-1].g = g; buf[j-3][i-1].b = b;
				}
				if(!flags[j-2][i-1]) {
					buf[j-2][i-1].r = r; buf[j-2][i-1].g = g; buf[j-2][i-1].b = b;
				}
				if(!flags[j-3][i]) {
					buf[j-3][i].r = r; buf[j-3][i].g = g; buf[j-3][i].b = b;
				}
				if(!flags[j-2][i]) {
					buf[j-2][i].r = r; buf[j-2][i].g = g; buf[j-2][i].b = b;
				}
				r = ((unsigned int)buf[j-3][i-3].r + (unsigned int)buf[j+3][i-3].r) >> 1;
				g = ((unsigned int)buf[j-3][i-3].g + (unsigned int)buf[j+3][i-3].g) >> 1;
				b = ((unsigned int)buf[j-3][i-3].b + (unsigned int)buf[j+3][i-3].b) >> 1;
				r = buf[j-3][i-3].r;
				g = buf[j-3][i-3].g;
				b = buf[j-3][i-3].b;
				if(!flags[j-1][i-3]) {
					buf[j-1][i-3].r = r; buf[j-1][i-3].g = g; buf[j-1][i-3].b = b;
				}
				if(!flags[j-1][i-2]) {
					buf[j-1][i-2].r = r; buf[j-1][i-2].g = g; buf[j-1][i-2].b = b;
				}
				if(!flags[j][i-3]) {
					buf[j][i-3].r = r; buf[j][i-3].g = g; buf[j][i-3].b = b;
				}
				if(!flags[j-1][i-2]) {
					buf[j][i-2].r = r; buf[j][i-2].g = g; buf[j][i-2].b = b;
				}
			} else {        /* else have to calc upper-left quad */
				for(i=x; i<x+4; i++) {
					for(j=0; j<4; j++) {
						if(!flags[j][i]) {
							flags[j][i] = 1;
							Shoot((Flt)i+0.5, (Flt)y+0.5+j, color);
							buf[j][i].r = (unsigned char)(255.0*color[0]);
							buf[j][i].g = (unsigned char)(255.0*color[1]);
							buf[j][i].b = (unsigned char)(255.0*color[2]);
						}
					}
				}
			}       /* end of upper-left quad */

			/* check upper right quad */
			i = x+3; j = (y%6)+3;   /* middle ray */

			if(comp(buf[j][i].r, buf[j-3][i+3].r) &&
			   comp(buf[j][i].g, buf[j-3][i+3].g) &&
			   comp(buf[j][i].b, buf[j-3][i+3].b)) { /* close enough so fill */
				if(!flags[j][i+1]) {
					buf[j][i+1].r = buf[j][i].r;
					buf[j][i+1].g = buf[j][i].g;
					buf[j][i+1].b = buf[j][i].b;
				}
				if(!flags[j-1][i+1]) {
					buf[j-1][i+1].r = buf[j][i].r;
					buf[j-1][i+1].g = buf[j][i].g;
					buf[j-1][i+1].b = buf[j][i].b;
				}
				if(!flags[j-2][i+2]) {
					buf[j-2][i+2].r = buf[j-3][i+3].r;
					buf[j-2][i+2].g = buf[j-3][i+3].g;
					buf[j-2][i+2].b = buf[j-3][i+3].b;
				}
				if(!flags[j-3][i+2]) {
					buf[j-3][i+2].r = buf[j-3][i+3].r;
					buf[j-3][i+2].g = buf[j-3][i+3].g;
					buf[j-3][i+2].b = buf[j-3][i+3].b;
				}
				if(!flags[j-2][i+3]) {
					buf[j-2][i+3].r = buf[j-3][i+3].r;
					buf[j-2][i+3].g = buf[j-3][i+3].g;
					buf[j-2][i+3].b = buf[j-3][i+3].b;
				}
				r = ((unsigned int)buf[j-3][i-3].r + (unsigned int)buf[j-3][i+3].r) >> 1;
				g = ((unsigned int)buf[j-3][i-3].g + (unsigned int)buf[j-3][i+3].g) >> 1;
				b = ((unsigned int)buf[j-3][i-3].b + (unsigned int)buf[j-3][i+3].b) >> 1;
				r = buf[j-3][i+3].r;
				g = buf[j-3][i+3].g;
				b = buf[j-3][i+3].b;
				if(!flags[j-3][i+1]) {
					buf[j-3][i+1].r = r; buf[j-3][i+1].g = g; buf[j-3][i+1].b = b;
				}
				if(!flags[j-2][i+1]) {
					buf[j-2][i+1].r = r; buf[j-2][i+1].g = g; buf[j-2][i+1].b = b;
				}
				r = ((unsigned int)buf[j-3][i+3].r + (unsigned int)buf[j+3][i+3].r) >> 1;
				g = ((unsigned int)buf[j-3][i+3].g + (unsigned int)buf[j+3][i+3].g) >> 1;
				b = ((unsigned int)buf[j-3][i+3].b + (unsigned int)buf[j+3][i+3].b) >> 1;
				r = buf[j-3][i+3].r;
				g = buf[j-3][i+3].g;
				b = buf[j-3][i+3].b;
				if(!flags[j-1][i+3]) {
					buf[j-1][i+3].r = r; buf[j-1][i+3].g = g; buf[j-1][i+3].b = b;
				}
				if(!flags[j-1][i+2]) {
					buf[j-1][i+2].r = r; buf[j-1][i+2].g = g; buf[j-1][i+2].b = b;
				}
				if(!flags[j][i+3]) {
					buf[j][i+3].r = r; buf[j][i+3].g = g; buf[j][i+3].b = b;
				}
				if(!flags[j-1][i+2]) {
					buf[j][i+2].r = r; buf[j][i+2].g = g; buf[j][i+2].b = b;
				}
			} else {        /* else have to calc upper-right quad */
				for(i=x+3; i<x+7; i++) {
					for(j=0; j<4; j++) {
						if(!flags[j][i]) {
							flags[j][i] = 1;
							Shoot((Flt)i+0.5, (Flt)y+0.5+j, color);
							buf[j][i].r = (unsigned char)(255.0*color[0]);
							buf[j][i].g = (unsigned char)(255.0*color[1]);
							buf[j][i].b = (unsigned char)(255.0*color[2]);
						}
					}
				}
			}       /* end of upper-right quad */

			/* handle lower left quad third */
			i = x+3; j = (y%6)+3;   /* middle ray */

			if(comp(buf[j][i].r, buf[j+3][i-3].r) &&
			   comp(buf[j][i].g, buf[j+3][i-3].g) &&
			   comp(buf[j][i].b, buf[j+3][i-3].b)) { /* close enough so fill */
				if(!flags[j+1][i]) {
					buf[j+1][i].r = buf[j][i].r;
					buf[j+1][i].g = buf[j][i].g;
					buf[j+1][i].b = buf[j][i].b;
				}
				if(!flags[j+1][i-1]) {
					buf[j+1][i-1].r = buf[j][i].r;
					buf[j+1][i-1].g = buf[j][i].g;
					buf[j+1][i-1].b = buf[j][i].b;
				}
				if(!flags[j+2][i-2]) {
					buf[j+2][i-2].r = buf[j+3][i-3].r;
					buf[j+2][i-2].g = buf[j+3][i-3].g;
					buf[j+2][i-2].b = buf[j+3][i-3].b;
				}
				if(!flags[j+3][i-2]) {
					buf[j+3][i-2].r = buf[j+3][i-3].r;
					buf[j+3][i-2].g = buf[j+3][i-3].g;
					buf[j+3][i-2].b = buf[j+3][i-3].b;
				}
				if(!flags[j+2][i-3]) {
					buf[j+2][i-3].r = buf[j+3][i-3].r;
					buf[j+2][i-3].g = buf[j+3][i-3].g;
					buf[j+2][i-3].b = buf[j+3][i-3].b;
				}
				r = ((unsigned int)buf[j+3][i-3].r + (unsigned int)buf[j+3][i+3].r) >> 1;
				g = ((unsigned int)buf[j+3][i-3].g + (unsigned int)buf[j+3][i+3].g) >> 1;
				b = ((unsigned int)buf[j+3][i-3].b + (unsigned int)buf[j+3][i+3].b) >> 1;
				r = buf[j+3][i-3].r;
				g = buf[j+3][i-3].g;
				b = buf[j+3][i-3].b;
				if(!flags[j+3][i-1]) {
					buf[j+3][i-1].r = r; buf[j+3][i-1].g = g; buf[j+3][i-1].b = b;
				}
				if(!flags[j+2][i-1]) {
					buf[j+2][i-1].r = r; buf[j+2][i-1].g = g; buf[j+2][i-1].b = b;
				}
				if(!flags[j+3][i]) {
					buf[j+3][i].r = r; buf[j+3][i].g = g; buf[j+3][i].b = b;
				}
				if(!flags[j+2][i]) {
					buf[j+2][i].r = r; buf[j+2][i].g = g; buf[j+2][i].b = b;
				}
				r = ((unsigned int)buf[j-3][i-3].r + (unsigned int)buf[j+3][i-3].r) >> 1;
				g = ((unsigned int)buf[j-3][i-3].g + (unsigned int)buf[j+3][i-3].g) >> 1;
				b = ((unsigned int)buf[j-3][i-3].b + (unsigned int)buf[j+3][i-3].b) >> 1;
				r = buf[j+3][i-3].r;
				g = buf[j+3][i-3].g;
				b = buf[j+3][i-3].b;
				if(!flags[j+1][i-3]) {
					buf[j+1][i-3].r = r; buf[j+1][i-3].g = g; buf[j+1][i-3].b = b;
				}
				if(!flags[j+1][i-2]) {
					buf[j+1][i-2].r = r; buf[j+1][i-2].g = g; buf[j+1][i-2].b = b;
				}
			} else {        /* else have to calc lower-left quad */
				for(i=x; i<x+4; i++) {
					for(j=3; j<7; j++) {
						if(!flags[j][i]) {
							flags[j][i] = 1;
							Shoot((Flt)i+0.5, (Flt)y+0.5+j, color);
							buf[j][i].r = (unsigned char)(255.0*color[0]);
							buf[j][i].g = (unsigned char)(255.0*color[1]);
							buf[j][i].b = (unsigned char)(255.0*color[2]);
						}
					}
				}
			}       /* end of lower-left quad */

			/* finally finish with lower right quad */
			i = x+3; j = (y%6)+3;   /* middle ray */

			if(comp(buf[j][i].r, buf[j+3][i+3].r) &&
			   comp(buf[j][i].g, buf[j+3][i+3].g) &&
			   comp(buf[j][i].b, buf[j+3][i+3].b)) { /* close enough so fill */
				if(!flags[j+1][i+1]) {
					buf[j+1][i+1].r = buf[j][i].r;
					buf[j+1][i+1].g = buf[j][i].g;
					buf[j+1][i+1].b = buf[j][i].b;
				}
				if(!flags[j+2][i+2]) {
					buf[j+2][i+2].r = buf[j+3][i+3].r;
					buf[j+2][i+2].g = buf[j+3][i+3].g;
					buf[j+2][i+2].b = buf[j+3][i+3].b;
				}
				if(!flags[j+3][i+2]) {
					buf[j+3][i+2].r = buf[j+3][i+3].r;
					buf[j+3][i+2].g = buf[j+3][i+3].g;
					buf[j+3][i+2].b = buf[j+3][i+3].b;
				}
				if(!flags[j+2][i-3]) {
					buf[j+2][i+3].r = buf[j+3][i+3].r;
					buf[j+2][i+3].g = buf[j+3][i+3].g;
					buf[j+2][i+3].b = buf[j+3][i+3].b;
				}
				r = ((unsigned int)buf[j+3][i-3].r + (unsigned int)buf[j+3][i+3].r) >> 1;
				g = ((unsigned int)buf[j+3][i-3].g + (unsigned int)buf[j+3][i+3].g) >> 1;
				b = ((unsigned int)buf[j+3][i-3].b + (unsigned int)buf[j+3][i+3].b) >> 1;
				r = buf[j+3][i+3].r;
				g = buf[j+3][i+3].g;
				b = buf[j+3][i+3].b;
				if(!flags[j+3][i+1]) {
					buf[j+3][i+1].r = r; buf[j+3][i+1].g = g; buf[j+3][i+1].b = b;
				}
				if(!flags[j+2][i+1]) {
					buf[j+2][i+1].r = r; buf[j+2][i+1].g = g; buf[j+2][i+1].b = b;
				}
				r = ((unsigned int)buf[j-3][i+3].r + (unsigned int)buf[j+3][i+3].r) >> 1;
				g = ((unsigned int)buf[j-3][i+3].g + (unsigned int)buf[j+3][i+3].g) >> 1;
				b = ((unsigned int)buf[j-3][i+3].b + (unsigned int)buf[j+3][i+3].b) >> 1;
				r = buf[j+3][i+3].r;
				g = buf[j+3][i+3].g;
				b = buf[j+3][i+3].b;
				if(!flags[j+1][i+3]) {
					buf[j+1][i+3].r = r; buf[j+1][i+3].g = g; buf[j+1][i+3].b = b;
				}
				if(!flags[j+1][i+2]) {
					buf[j+1][i+2].r = r; buf[j+1][i+2].g = g; buf[j+1][i+2].b = b;
				}
			} else {        /* else have to calc lower-right quad */
				for(i=x+3; i<x+7; i++) {
					for(j=3; j<7; j++) {
						if(!flags[j][i]) {
							flags[j][i] = 1;
							Shoot((Flt)i+0.5, (Flt)y+0.5+j, color);
							buf[j][i].r = (unsigned char)(255.0*color[0]);
							buf[j][i].g = (unsigned char)(255.0*color[1]);
							buf[j][i].b = (unsigned char)(255.0*color[2]);
						}
					}
				}
			}       /* end of lower-right quad */

		}       /* end of x loop */

		/* output scans */
		for(j=0; j<6; j++) {
			PicWriteLine(pic, buf[j]);
		}
		if(tickflag)
			statistics(y+6);
	}       /* end of y loop (finally!) */
	if(tickflag)
		fprintf(stderr, "\n");
}       /* end of Scan2() */

/*
	comp - compares two numbers, returns 1 if close enough, 0 otherwise
*/

int comp(unsigned int a, unsigned int b)
{
	int diff;

	diff = a - b;
	if(diff > adapt_dist)
		return 0;
	else if(diff < -adapt_dist)
		return 0;
	else
		return 1;
}       /* end of comp() */

/*
	Scan3 -- Adaptive supersampling with optional jitter.
		 Jitter +- 0.125
*/

#define SIDE		(4)

#define RAW		(0)
#define COOKED          (1)
#define FUZZY           (2)

static unsigned char	win[SIDE+1][SIDE+1][4];	/* r,g,b,flag */
static unsigned char	*buff[4];

void    Scan3(void)
{
	Pixel	*buf;
	Color	color;		/* color of current traced ray */
	int     x, y, i, j;

	buf = (Pixel *)malloc(x_res * sizeof(Pixel));
	ptrchk(buf, "output buffer.");

	for(x=0; x<4; x++) {
		buff[x] = (unsigned char *)malloc(SIDE*x_res+1);
		ptrchk(buff[x], "antialiasing buffer.");
	}

	for(i=0; i<SIDE+1; i++)	{		/* clear win flags */
		for(j=0; j<SIDE+1; j++) {
			win[i][j][3] = RAW;
		}
	}
	for(i=0; i<SIDE*x_res+1; i++) {		/* clear buff flags */
		buff[3][i] = RAW;
	}

	for(y=start_line; y<stop_line; y++) {

		/* clear left edge of win for starting a new row */
		for(j=0; j<SIDE+1; j++) {
			win[0][j][3] = RAW;
		}

		for(x=0; x<x_res; x++) {

			for(i=1; i<SIDE+1; i++)	/* buff to top row of win */
				if(win[i][0][3] = buff[3][x*SIDE+i]) {	/* if cooked */
					win[i][0][0] = buff[0][x*SIDE+i];
					win[i][0][1] = buff[1][x*SIDE+i];
					win[i][0][2] = buff[2][x*SIDE+i];
				}

			for(i=1; i<SIDE+1; i++)	/* clear rest of win */
				for(j=1; j<SIDE+1; j++)
					win[i][j][3] = RAW;

			Adapt(0, 0, (Flt)x, (Flt)y, color, SIDE);

			buf[x].r = (unsigned char) (color[0]);
			buf[x].g = (unsigned char) (color[1]);
			buf[x].b = (unsigned char) (color[2]);

			for(i=0; i<SIDE+1; i++)	/* bottom row of win to buff */
				if(buff[3][x*SIDE+i] = win[i][SIDE][3]) {	/* if cooked */
					buff[0][x*SIDE+i] = win[i][SIDE][0];
					buff[1][x*SIDE+i] = win[i][SIDE][1];
					buff[2][x*SIDE+i] = win[i][SIDE][2];
				}

			for(j=0; j<SIDE+1; j++)	{	/* right edge of win to left */
				if(win[0][j][3] = win[SIDE][j][3]) {	/* if cooked */
					win[0][j][0] = win[SIDE][j][0];
					win[0][j][1] = win[SIDE][j][1];
					win[0][j][2] = win[SIDE][j][2];
				}
			}
		}
		PicWriteLine(pic, buf);
		if(tickflag)
			statistics(y);
	}
	if(tickflag)
		fprintf(stderr, "\n");

	free(buf);
	for(i=0; i<4; i++)
		if(buff[i])
			free(buff[i]);
	
}       /* end of Scan3() */

#define ARAND() (((rand()/(Flt)RAND_MAX)/4.0)-0.125)

void	Adapt(int i, int j, Flt x, Flt y, Color color, int step)
//	int	i, j;		/* where in win to put results */
//	Flt	x, y;		/* upper left hand of pixel */
//	Color	color;		/* return pixel color here in 0..255 range */
//	int	step;		/* what level we're at */
{
	int     k, fuzzed;
	int     ave[3], c0[3], c1[3], c2[3], c3[3];

	if(win[i][j][3] == RAW) {
		if(jitter) {
			Shoot(x+(Flt)i/SIDE+ARAND(), y+(Flt)j/SIDE+ARAND(), color);
		} else {
			Shoot(x+(Flt)i/SIDE, y+(Flt)j/SIDE, color);
		}

		c0[0] = win[i][j][0] = (unsigned char) (255.0 * color[0]);
		c0[1] = win[i][j][1] = (unsigned char) (255.0 * color[1]);
		c0[2] = win[i][j][2] = (unsigned char) (255.0 * color[2]);
		if(fuzzy_ray) {
			win[i][j][3] = COOKED | FUZZY;
		} else {
			win[i][j][3] = COOKED;
		}
	} else {
		c0[0] = win[i][j][0];
		c0[1] = win[i][j][1];
		c0[2] = win[i][j][2];
	}

	if(win[i+step][j][3] == RAW) {
		if(jitter) {
			Shoot((Flt)x+(Flt)(i+step)/SIDE+ARAND(), (Flt)y+(Flt)j/SIDE+ARAND(), color);
		} else {
			Shoot((Flt)x+(Flt)(i+step)/SIDE, (Flt)y+(Flt)j/SIDE, color);
		}

		c1[0] = win[i+step][j][0] = (unsigned char) (255.0 * color[0]);
		c1[1] = win[i+step][j][1] = (unsigned char) (255.0 * color[1]);
		c1[2] = win[i+step][j][2] = (unsigned char) (255.0 * color[2]);
		if(fuzzy_ray) {
			win[i+step][j][3] = COOKED | FUZZY;
		} else {
			win[i+step][j][3] = COOKED;
		}
	} else {
		c1[0] = win[i+step][j][0];
		c1[1] = win[i+step][j][1];
		c1[2] = win[i+step][j][2];
	}

	if(win[i][j+step][3] == RAW) {
		if(jitter) {
			Shoot((Flt)x+(Flt)i/SIDE+ARAND(), (Flt)y+(Flt)(j+step)/SIDE+ARAND(), color);
		} else {
			Shoot((Flt)x+(Flt)i/SIDE, (Flt)y+(Flt)(j+step)/SIDE, color);
		}

		c2[0] = win[i][j+step][0] = (unsigned char) (255.0 * color[0]);
		c2[1] = win[i][j+step][1] = (unsigned char) (255.0 * color[1]);
		c2[2] = win[i][j+step][2] = (unsigned char) (255.0 * color[2]);
		if(fuzzy_ray) {
			win[i][j+step][3] = COOKED | FUZZY;
		} else {
			win[i][j+step][3] = COOKED;
		}
	} else {
		c2[0] = win[i][j+step][0];
		c2[1] = win[i][j+step][1];
		c2[2] = win[i][j+step][2];
	}

	if(win[i+step][j+step][3] == RAW) {
		if(jitter) {
			Shoot((Flt)x+(Flt)(i+step)/SIDE+ARAND(), (Flt)y+(Flt)(j+step)/SIDE+ARAND(), color);
		} else {
			Shoot((Flt)x+(Flt)(i+step)/SIDE, (Flt)y+(Flt)(j+step)/SIDE, color);
		}

		c3[0] = win[i+step][j+step][0] = (unsigned char) (255.0 * color[0]);
		c3[1] = win[i+step][j+step][1] = (unsigned char) (255.0 * color[1]);
		c3[2] = win[i+step][j+step][2] = (unsigned char) (255.0 * color[2]);
		if(fuzzy_ray) {
			win[i+step][j+step][3] = COOKED | FUZZY;
		} else {
			win[i+step][j+step][3] = COOKED;
		}
	} else {
		c3[0] = win[i+step][j+step][0];
		c3[1] = win[i+step][j+step][1];
		c3[2] = win[i+step][j+step][2];
	}

	for(k=0; k<3; k++) {
		ave[k] = c0[k] + c1[k] + c2[k] + c3[k];
		ave[k] /= 4;
	}

	fuzzed = win[i][j][3] &
		win[i+step][j][3] &
		win[i][j+step][3] &
		win[i+step][j+step][3] &
		FUZZY;

	if(step==1 || fuzzed ||
		ABS(ave[0] - c0[0]) < adapt_dist &&
		ABS(ave[1] - c0[1]) < adapt_dist &&
		ABS(ave[2] - c0[2]) < adapt_dist &&
		ABS(ave[0] - c1[0]) < adapt_dist &&
		ABS(ave[1] - c1[1]) < adapt_dist &&
		ABS(ave[2] - c1[2]) < adapt_dist &&
		ABS(ave[0] - c2[0]) < adapt_dist &&
		ABS(ave[1] - c2[1]) < adapt_dist &&
		ABS(ave[2] - c2[2]) < adapt_dist &&
		ABS(ave[0] - c3[0]) < adapt_dist &&
		ABS(ave[1] - c3[1]) < adapt_dist &&
		ABS(ave[2] - c3[2]) < adapt_dist) {     /* close enough */
		color[0] = ave[0];
		color[1] = ave[1];
		color[2] = ave[2];

		return;
	}

	/* not close, so we have to subdivide */

	step /= 2;
	ave[0] = 0;
	ave[1] = 0;
	ave[2] = 0;

	Adapt(i, j, (Flt)x, (Flt)y, color, step);
	ave[0] += (unsigned char) (color[0]);
	ave[1] += (unsigned char) (color[1]);
	ave[2] += (unsigned char) (color[2]);

	Adapt(i+step, j, (Flt)x, (Flt)y, color, step);
	ave[0] += (unsigned char) (color[0]);
	ave[1] += (unsigned char) (color[1]);
	ave[2] += (unsigned char) (color[2]);

	Adapt(i, j+step, (Flt)x, (Flt)y, color, step);
	ave[0] += (unsigned char) (color[0]);
	ave[1] += (unsigned char) (color[1]);
	ave[2] += (unsigned char) (color[2]);

	Adapt(i+step, j+step, (Flt)x, (Flt)y, color, step);
	ave[0] += (unsigned char) (color[0]);
	ave[1] += (unsigned char) (color[1]);
	ave[2] += (unsigned char) (color[2]);

	color[0] = ave[0]>>2;
	color[1] = ave[1]>>2;
	color[2] = ave[2]>>2;

	return;
}	/* end of Adapt() */

void	Shoot(Flt x, Flt y, Color color)
//	Flt		x, y;		/* where on screen to shoot */
//	Color		color;		/* color to return from shot */
{
	Color   sum_color;      /* summed color for DOF effects */
	Flt	random;
	Ray	ray2;		/* ray tweeked for non-pinhole cameras */
	Vec	dir;
	int	sample;
	Flt     tx, ty, scale, P;

	switch(camera.projection) {
		case P_FLAT :
		case P_NO_PARALLAX:
			VecComb(-frustrumheight*(2.0*y/(Flt)y_res - 1.0),
				looking_up,
				frustrumwidth*(2.0*x/(Flt)x_res - 1.0),
				leftvec, dir);
			VecAdd(dir, viewvec, ray.D);
			VecNormalize(ray.D);
			break;
		case P_FISHEYE :
			tx = (x-x_res/2.0)/(Flt)x_res*Eye.view_angle_x*2.0;
			ty = -(y-y_res/2.0)/(Flt)y_res*Eye.view_angle_y*2.0;

			VecComb(sin(ty), looking_up, sin(tx), leftvec, dir);
			VecAddS(cos(tx)*cos(ty), viewvec, dir, ray.D);
			VecNormalize(ray.D);
			break;
		case P_ORTHOGRAPHIC :
			VecComb(-Eye.view_angle_y*(2.0*y/(Flt)y_res - 1.0),
				looking_up,
				Eye.view_angle_x*(2.0*x/(Flt)x_res - 1.0),
				leftvec, dir);
			VecAdd(dir, viewpoint, ray.P);
			break;
	}       /* end of projection switch */

	fuzzy_ray = 0;
	if(camera.aperture > 0.0) {
		MakeVector(0, 0, 0, sum_color);
		for(sample=0; sample<camera.samples; sample++) {
			dir[0] = ray.P[0] + ray.D[0]*camera.focal_length;
			dir[1] = ray.P[1] + ray.D[1]*camera.focal_length;
			dir[2] = ray.P[2] + ray.D[2]*camera.focal_length;
			VecCopy(ray.P, ray2.P);
			random = rnd();
			if(rnd() > 0.5) random = -random;
			VecAddS(random, camera.lens_i, ray2.P, ray2.P);
			random = rnd();
			if(rnd() > 0.5) random = -random;
			VecAddS(random, camera.lens_j, ray2.P, ray2.P);
			VecSub(dir, ray2.P, ray2.D);
			VecNormalize(ray2.D);					

			Trace(0, 1.0, &ray2, color, 1.0, NULL);
			if (color[0] > 1.0) color[0] = 1.0;
			if (color[1] > 1.0) color[1] = 1.0;
			if (color[2] > 1.0) color[2] = 1.0;
			VecAdd(color, sum_color, sum_color);
		}
		VecS((1.0/(Flt)camera.samples), sum_color, color);
	} else {
		Trace(0, 1.0, &ray, color, 1.0, NULL);
		if (color[0] > 1.0) color[0] = 1.0;
		if (color[1] > 1.0) color[1] = 1.0;
		if (color[2] > 1.0) color[2] = 1.0;
	}
}	/* end of shoot */



