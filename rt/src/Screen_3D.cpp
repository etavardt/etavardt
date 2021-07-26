/*
***************************************************************************
*                                                                         *
*                             Bob Ray Tracer                              *
*                                                                         *
*                Screen.C = screen scanning and anti-aliasing             *
*                                                                         *
*       Copyright 1988,1992 Christopher D. Watkins and Stephen B. Coy     *
*                                                                         *
*       ALL RIGHTS RESERVED.   This software is published, but is NOT     *
*         Public Domain and remains the propery of ALGORITHM, Inc.,       *
*   Christopher D. Watkins and Stephen B. Coy.  This software may not be  *
*  reproduced or integrated into other packages without the prior written *
*          consent of Christopher D. Watkins and Stephen B. Coy.          *
*                                                                         *
*                    Requires: defs.h, pic.h, extern.h                    *
*                                                                         *
***************************************************************************

    screen -- Contains the scan functions which determine what
        antialiasing is done and the function shoot() which
        handle the projection mode, depth of field and
        actually calling the trace function.
*/

#include "Screen_3D.hpp"

#include <cmath>

#include "Bob.hpp"
#include "BobMath.hpp"
#include "RayTrace_3D.hpp"
#include "Vector_3D.hpp"
#include "PicFile_3D.hpp"
#include "Color.hpp"
#include "Stats.hpp"
#include "extern.hpp"

extern int tickflag;


Screen_3D::Screen_3D(Camera_3D &cam) : camera(cam) {
    picFile = new PicFile_3D();
}
Screen_3D::~Screen_3D() {
    if (picFile != nullptr) {
        picFile->close();
        delete picFile;
    }
}

void Screen_3D::screen(String &picfile, int xres, int yres) {
    scrInit(xres, yres, picfile);

    switch (antialias) {
    case A_NONE:
        scan0();
        break;
    case A_CORNERS:
        scan1();
        break;
    case A_QUICK:
        scan2();
        break;
    case A_ADAPTIVE:
        scan3();
        break;
    }
    picFile->close();
}

void Screen_3D::scrInit(int xres, int yres, String &picFileName) {
    // open the picture file...
    picFile->open(picFileName, xres, yres);

    // determine the viewing frustrum
    x_res = xres;
    y_res = yres;

    VecNormalize(camera.eye.view_up);
    VecNormalize(camera.eye.view_up);
    VecSub(camera.eye.view_at, camera.eye.view_from, viewvec);
    VecNormalize(viewvec);
    VecCross(camera.eye.view_up, viewvec, leftvec);
    VecNormalize(leftvec);
    VecS((-1), leftvec, leftvec); /* convert to right handed */

    /* make view_up vector perpendicular to veiwvec and leftvec */

    if (camera.projection != P_NO_PARALLAX) {
        VecCross(leftvec, viewvec, camera.eye.view_up);
        VecNormalize(camera.eye.view_up);
    }
    VecCopy(camera.eye.view_up, looking_up);

    /* set up camera stuff */

    if (camera.aperture > 0.0) { /* calc lens vectors */
        VecCopy(leftvec, camera.lens_i);
        VecCross(viewvec, camera.lens_i, camera.lens_j);
        VecNormalize(camera.lens_j);
        VecS(camera.aperture, camera.lens_i, camera.lens_i);
        VecS(camera.aperture, camera.lens_j, camera.lens_j);
    }

    VecCopy(camera.eye.view_from, ray.P);
    VecCopy(camera.eye.view_from, viewpoint);
    VecCopy(viewvec, ray.D);

    frustrumwidth = tan(camera.eye.view_angle_x);
    frustrumheight = tan(camera.eye.view_angle_y);

} /* end of ScrInit() */

/*
    comp - compares two numbers, returns 1 if close enough, 0 otherwise
    used in scan2 only
*/
int Screen_3D::comp(unsigned int a, unsigned int b) {
    int diff;

    diff = a - b;
    if (diff > adapt_dist)
        return 0;
    else if (diff < -adapt_dist)
        return 0;
    else
        return 1;
} /* end of comp() */

/*
    Scan0 -- Basic one ray per pixel, right through the center.
         Jitter value +- 0.5
*/
void Screen_3D::scan0(void) {
    Pixel *buf;
    Color color; /* color of current traced ray */
    int i, j;
    double x, y;

    buf = new Pixel[x_res]();

    for (j = start_line; j < stop_line; j++) {
        for (i = 0; i < x_res; i++) {
            if (jitter) {
                x = i + rand() / (double)RAND_MAX;
                y = j + rand() / (double)RAND_MAX;
            } else {
                x = i + 0.5; /* hit center of pixel */
                y = j + 0.5;
            }

            shoot(x, y, color);
            buf[i] = color.getPixelColor();
        }
        picFile->writeLine(buf);
        if (tickflag)
            Stats::statistics(j);
    }
    if (tickflag)
        fprintf(stderr, "\n");

    free(buf);
} /* end of Scan0() */

/*
    Scan1 -- shoot a ray at each corner of the pixel and average
         the results.
         Jitter value +- 0.5
*/
void Screen_3D::scan1(void) {
    Pixel *buf, *oldbuf, *curbuf, *tmp;
    int red, green, blue;
    Color color; /* color of current traced ray */
    int i, j;
    double x, y;

    /*
     * allocate enough space for an entire row of pixels...
     * plus one more for the average...
     */

    oldbuf = NULL;
    curbuf = new Pixel[x_res + 1](); // Why +1
    buf = new Pixel[x_res + 1]();    // Why +1

    for (j = start_line; j < stop_line; j++) {
        for (i = 0; i < x_res + 1; i++) {
            if (jitter) {
                x = i + rand() / (double)RAND_MAX;
                y = j + rand() / (double)RAND_MAX;
            } else {
                x = i + 0.5; /* hit center of pixel */
                y = j + 0.5;
            }
            shoot(x, y, color);
            curbuf[i] = color.getPixelColor();
        }
        if (oldbuf) {
            /* average the pixels, and write 'em out */
            for (i = 0; i < x_res; i++) {
                red = ((int)curbuf[i].r + (int)curbuf[i + 1].r + (int)oldbuf[i].r + (int)oldbuf[i + 1].r);
                green = ((int)curbuf[i].g + (int)curbuf[i + 1].g + (int)oldbuf[i].g + (int)oldbuf[i + 1].g);
                blue = ((int)curbuf[i].b + (int)curbuf[i + 1].b + (int)oldbuf[i].b + (int)oldbuf[i + 1].b);
                buf[i].r = (unsigned char)(red / 4);
                buf[i].g = (unsigned char)(green / 4);
                buf[i].b = (unsigned char)(blue / 4);
            }
            picFile->writeLine(buf);
            tmp = oldbuf;
            oldbuf = curbuf;
            curbuf = tmp;
        } else {
            oldbuf = curbuf;
            curbuf = new Pixel[x_res + 1](); // Why +1
        }

        if (tickflag)
            Stats::statistics(j);
    }
    if (tickflag)
        fprintf(stderr, "\n");

    free(buf);
    free(curbuf);
} /* end of Scan1() */

/*
    Scan2 -- Undersampling, possible 18 to 1 speedup.
        No jitter option.
*/
void Screen_3D::scan2(void) {
    Pixel *buf[7], *pixelBuf;
    int *flags[7];
    Color color; /* color of current traced ray */
    int x, y, xx, yy, i, j;

    unsigned int r, g, b;

    /* fill to next mod 6 scan line */
    if (start_line % 6) {
        pixelBuf = new Pixel[x_res](); // Why +1


        /* calc stop line for single fill */
        yy = start_line + 6 - (start_line % 6);
        for (j = start_line; j < yy; j++) {
            for (i = 0; i < x_res; i++) {
                shoot((double)i + 0.5, (double)j + 0.5, color);
                pixelBuf[i] = color.getPixelColor();
            }
            picFile->writeLine(pixelBuf);
            if (tickflag)
                Stats::statistics(j);
        }
        start_line = yy;
        //free(pixelBuf);
        delete pixelBuf;
    }

    /* allocate pixel buffers */

    for (i = 0; i < 7; i++) {
        buf[i] = new Pixel[x_res + 5](); // Why +5

        flags[i] = new int[x_res + 5](); // Why +5

    }

    /* start actual sub-sampling */

    for (i = 0; i < x_res + 5; i++) { /* clear bottom row of flags */
        flags[6][i] = 0;
    }
    for (y = start_line; y < stop_line; y += 6) {

        /* copy bottom line to top */
        for (i = 0; i < x_res + 5; i++) {
            if (flags[0][i] = flags[6][i]) { /* only copy if valid *///TCE: Is this a valid assignment or should it be an == test
                buf[0][i].r = buf[6][i].r;
                buf[0][i].g = buf[6][i].g;
                buf[0][i].b = buf[6][i].b;
            }
            /* clear rest of buf */
            for (j = 1; j < 7; j++) {
                flags[j][i] = 0;
            }
        }

        /* for(x=0; x<x_res+5; x+=6) { */
        for (x = 0; x < x_res; x += 6) {
            /* shoot corners and middle */
            i = x;
            j = 0;
            if (!flags[j][i]) {
                flags[j][i] = 1;
                shoot((double)i + 0.5, (double)y + 0.5, color);
                buf[j][i] = color.getPixelColor();
            }

            i = x + 6;
            j = 0;
            if (!flags[j][i]) {
                flags[j][i] = 1;
                shoot((double)i + 0.5, (double)y + 0.5, color);
                buf[j][i] = color.getPixelColor();
            }

            i = x;
            j = 6;
            if (!flags[j][i]) {
                flags[j][i] = 1;
                shoot((double)i + 0.5, (double)y + 0.5 + 6, color);
                buf[j][i] = color.getPixelColor();
            }

            i = x + 6;
            j = 6;
            if (!flags[j][i]) {
                flags[j][i] = 1;
                shoot((double)i + 0.5, (double)y + 0.5 + 6, color);
                buf[j][i] = color.getPixelColor();
            }

            i = x + 3;
            j = 3; /* middle ray */
            if (!flags[j][i]) {
                flags[j][i] = 1;
                shoot((double)i + 0.5, (double)y + 0.5 + 3, color);
                buf[j][i] = color.getPixelColor();
            }

            /* the corners are shot, now fill in if needed */

            /* check upper left quad first */
            i = x + 3;
            j = (y % 6) + 3; /* middle ray */

            if (comp(buf[j][i].r, buf[j - 3][i - 3].r)
            &&  comp(buf[j][i].g, buf[j - 3][i - 3].g)
            &&  comp(buf[j][i].b, buf[j - 3][i - 3].b)) { /* close enough so fill */
                if (!flags[j - 1][i]) {
                    buf[j - 1][i].r = buf[j][i].r;
                    buf[j - 1][i].g = buf[j][i].g;
                    buf[j - 1][i].b = buf[j][i].b;
                }
                if (!flags[j][i - 1]) {
                    buf[j][i - 1].r = buf[j][i].r;
                    buf[j][i - 1].g = buf[j][i].g;
                    buf[j][i - 1].b = buf[j][i].b;
                }
                if (!flags[j - 1][i - 1]) {
                    buf[j - 1][i - 1].r = buf[j][i].r;
                    buf[j - 1][i - 1].g = buf[j][i].g;
                    buf[j - 1][i - 1].b = buf[j][i].b;
                }
                if (!flags[j - 2][i - 2]) {
                    buf[j - 2][i - 2].r = buf[j - 3][i - 3].r;
                    buf[j - 2][i - 2].g = buf[j - 3][i - 3].g;
                    buf[j - 2][i - 2].b = buf[j - 3][i - 3].b;
                }
                if (!flags[j - 3][i - 2]) {
                    buf[j - 3][i - 2].r = buf[j - 3][i - 3].r;
                    buf[j - 3][i - 2].g = buf[j - 3][i - 3].g;
                    buf[j - 3][i - 2].b = buf[j - 3][i - 3].b;
                }
                if (!flags[j - 2][i - 3]) {
                    buf[j - 2][i - 3].r = buf[j - 3][i - 3].r;
                    buf[j - 2][i - 3].g = buf[j - 3][i - 3].g;
                    buf[j - 2][i - 3].b = buf[j - 3][i - 3].b;
                }
                r = ((unsigned int)buf[j - 3][i - 3].r + (unsigned int)buf[j - 3][i + 3].r) >> 1;
                g = ((unsigned int)buf[j - 3][i - 3].g + (unsigned int)buf[j - 3][i + 3].g) >> 1;
                b = ((unsigned int)buf[j - 3][i - 3].b + (unsigned int)buf[j - 3][i + 3].b) >> 1;
                r = buf[j - 3][i - 3].r;
                g = buf[j - 3][i - 3].g;
                b = buf[j - 3][i - 3].b;
                if (!flags[j - 3][i - 1]) {
                    buf[j - 3][i - 1].r = r;
                    buf[j - 3][i - 1].g = g;
                    buf[j - 3][i - 1].b = b;
                }
                if (!flags[j - 2][i - 1]) {
                    buf[j - 2][i - 1].r = r;
                    buf[j - 2][i - 1].g = g;
                    buf[j - 2][i - 1].b = b;
                }
                if (!flags[j - 3][i]) {
                    buf[j - 3][i].r = r;
                    buf[j - 3][i].g = g;
                    buf[j - 3][i].b = b;
                }
                if (!flags[j - 2][i]) {
                    buf[j - 2][i].r = r;
                    buf[j - 2][i].g = g;
                    buf[j - 2][i].b = b;
                }
                r = ((unsigned int)buf[j - 3][i - 3].r + (unsigned int)buf[j + 3][i - 3].r) >> 1;
                g = ((unsigned int)buf[j - 3][i - 3].g + (unsigned int)buf[j + 3][i - 3].g) >> 1;
                b = ((unsigned int)buf[j - 3][i - 3].b + (unsigned int)buf[j + 3][i - 3].b) >> 1;
                r = buf[j - 3][i - 3].r;
                g = buf[j - 3][i - 3].g;
                b = buf[j - 3][i - 3].b;
                if (!flags[j - 1][i - 3]) {
                    buf[j - 1][i - 3].r = r;
                    buf[j - 1][i - 3].g = g;
                    buf[j - 1][i - 3].b = b;
                }
                if (!flags[j - 1][i - 2]) {
                    buf[j - 1][i - 2].r = r;
                    buf[j - 1][i - 2].g = g;
                    buf[j - 1][i - 2].b = b;
                }
                if (!flags[j][i - 3]) {
                    buf[j][i - 3].r = r;
                    buf[j][i - 3].g = g;
                    buf[j][i - 3].b = b;
                }
                if (!flags[j - 1][i - 2]) {
                    buf[j][i - 2].r = r;
                    buf[j][i - 2].g = g;
                    buf[j][i - 2].b = b;
                }
            } else { /* else have to calc upper-left quad */
                for (i = x; i < x + 4; i++) {
                    for (j = 0; j < 4; j++) {
                        if (!flags[j][i]) {
                            flags[j][i] = 1;
                            shoot((double)i + 0.5, (double)y + 0.5 + j, color);
                            buf[j][i] = color.getPixelColor();
                        }
                    }
                }
            } /* end of upper-left quad */

            /* check upper right quad */
            i = x + 3;
            j = (y % 6) + 3; /* middle ray */

            if (comp(buf[j][i].r, buf[j - 3][i + 3].r) && comp(buf[j][i].g, buf[j - 3][i + 3].g) && comp(buf[j][i].b, buf[j - 3][i + 3].b)) { /* close enough so fill */
                if (!flags[j][i + 1]) {
                    buf[j][i + 1].r = buf[j][i].r;
                    buf[j][i + 1].g = buf[j][i].g;
                    buf[j][i + 1].b = buf[j][i].b;
                }
                if (!flags[j - 1][i + 1]) {
                    buf[j - 1][i + 1].r = buf[j][i].r;
                    buf[j - 1][i + 1].g = buf[j][i].g;
                    buf[j - 1][i + 1].b = buf[j][i].b;
                }
                if (!flags[j - 2][i + 2]) {
                    buf[j - 2][i + 2].r = buf[j - 3][i + 3].r;
                    buf[j - 2][i + 2].g = buf[j - 3][i + 3].g;
                    buf[j - 2][i + 2].b = buf[j - 3][i + 3].b;
                }
                if (!flags[j - 3][i + 2]) {
                    buf[j - 3][i + 2].r = buf[j - 3][i + 3].r;
                    buf[j - 3][i + 2].g = buf[j - 3][i + 3].g;
                    buf[j - 3][i + 2].b = buf[j - 3][i + 3].b;
                }
                if (!flags[j - 2][i + 3]) {
                    buf[j - 2][i + 3].r = buf[j - 3][i + 3].r;
                    buf[j - 2][i + 3].g = buf[j - 3][i + 3].g;
                    buf[j - 2][i + 3].b = buf[j - 3][i + 3].b;
                }
                r = ((unsigned int)buf[j - 3][i - 3].r + (unsigned int)buf[j - 3][i + 3].r) >> 1;
                g = ((unsigned int)buf[j - 3][i - 3].g + (unsigned int)buf[j - 3][i + 3].g) >> 1;
                b = ((unsigned int)buf[j - 3][i - 3].b + (unsigned int)buf[j - 3][i + 3].b) >> 1;
                r = buf[j - 3][i + 3].r;
                g = buf[j - 3][i + 3].g;
                b = buf[j - 3][i + 3].b;
                if (!flags[j - 3][i + 1]) {
                    buf[j - 3][i + 1].r = r;
                    buf[j - 3][i + 1].g = g;
                    buf[j - 3][i + 1].b = b;
                }
                if (!flags[j - 2][i + 1]) {
                    buf[j - 2][i + 1].r = r;
                    buf[j - 2][i + 1].g = g;
                    buf[j - 2][i + 1].b = b;
                }
                r = ((unsigned int)buf[j - 3][i + 3].r + (unsigned int)buf[j + 3][i + 3].r) >> 1;
                g = ((unsigned int)buf[j - 3][i + 3].g + (unsigned int)buf[j + 3][i + 3].g) >> 1;
                b = ((unsigned int)buf[j - 3][i + 3].b + (unsigned int)buf[j + 3][i + 3].b) >> 1;
                r = buf[j - 3][i + 3].r;
                g = buf[j - 3][i + 3].g;
                b = buf[j - 3][i + 3].b;
                if (!flags[j - 1][i + 3]) {
                    buf[j - 1][i + 3].r = r;
                    buf[j - 1][i + 3].g = g;
                    buf[j - 1][i + 3].b = b;
                }
                if (!flags[j - 1][i + 2]) {
                    buf[j - 1][i + 2].r = r;
                    buf[j - 1][i + 2].g = g;
                    buf[j - 1][i + 2].b = b;
                }
                if (!flags[j][i + 3]) {
                    buf[j][i + 3].r = r;
                    buf[j][i + 3].g = g;
                    buf[j][i + 3].b = b;
                }
                if (!flags[j - 1][i + 2]) {
                    buf[j][i + 2].r = r;
                    buf[j][i + 2].g = g;
                    buf[j][i + 2].b = b;
                }
            } else { /* else have to calc upper-right quad */
                for (i = x + 3; i < x + 7; i++) {
                    for (j = 0; j < 4; j++) {
                        if (!flags[j][i]) {
                            flags[j][i] = 1;
                            shoot((double)i + 0.5, (double)y + 0.5 + j, color);
                            buf[j][i] = color.getPixelColor();
                        }
                    }
                }
            } /* end of upper-right quad */

            /* handle lower left quad third */
            i = x + 3;
            j = (y % 6) + 3; /* middle ray */

            if (comp(buf[j][i].r, buf[j + 3][i - 3].r) && comp(buf[j][i].g, buf[j + 3][i - 3].g) && comp(buf[j][i].b, buf[j + 3][i - 3].b)) { /* close enough so fill */
                if (!flags[j + 1][i]) {
                    buf[j + 1][i].r = buf[j][i].r;
                    buf[j + 1][i].g = buf[j][i].g;
                    buf[j + 1][i].b = buf[j][i].b;
                }
                if (!flags[j + 1][i - 1]) {
                    buf[j + 1][i - 1].r = buf[j][i].r;
                    buf[j + 1][i - 1].g = buf[j][i].g;
                    buf[j + 1][i - 1].b = buf[j][i].b;
                }
                if (!flags[j + 2][i - 2]) {
                    buf[j + 2][i - 2].r = buf[j + 3][i - 3].r;
                    buf[j + 2][i - 2].g = buf[j + 3][i - 3].g;
                    buf[j + 2][i - 2].b = buf[j + 3][i - 3].b;
                }
                if (!flags[j + 3][i - 2]) {
                    buf[j + 3][i - 2].r = buf[j + 3][i - 3].r;
                    buf[j + 3][i - 2].g = buf[j + 3][i - 3].g;
                    buf[j + 3][i - 2].b = buf[j + 3][i - 3].b;
                }
                if (!flags[j + 2][i - 3]) {
                    buf[j + 2][i - 3].r = buf[j + 3][i - 3].r;
                    buf[j + 2][i - 3].g = buf[j + 3][i - 3].g;
                    buf[j + 2][i - 3].b = buf[j + 3][i - 3].b;
                }
                r = ((unsigned int)buf[j + 3][i - 3].r + (unsigned int)buf[j + 3][i + 3].r) >> 1;
                g = ((unsigned int)buf[j + 3][i - 3].g + (unsigned int)buf[j + 3][i + 3].g) >> 1;
                b = ((unsigned int)buf[j + 3][i - 3].b + (unsigned int)buf[j + 3][i + 3].b) >> 1;
                r = buf[j + 3][i - 3].r;
                g = buf[j + 3][i - 3].g;
                b = buf[j + 3][i - 3].b;
                if (!flags[j + 3][i - 1]) {
                    buf[j + 3][i - 1].r = r;
                    buf[j + 3][i - 1].g = g;
                    buf[j + 3][i - 1].b = b;
                }
                if (!flags[j + 2][i - 1]) {
                    buf[j + 2][i - 1].r = r;
                    buf[j + 2][i - 1].g = g;
                    buf[j + 2][i - 1].b = b;
                }
                if (!flags[j + 3][i]) {
                    buf[j + 3][i].r = r;
                    buf[j + 3][i].g = g;
                    buf[j + 3][i].b = b;
                }
                if (!flags[j + 2][i]) {
                    buf[j + 2][i].r = r;
                    buf[j + 2][i].g = g;
                    buf[j + 2][i].b = b;
                }
                r = ((unsigned int)buf[j - 3][i - 3].r + (unsigned int)buf[j + 3][i - 3].r) >> 1;
                g = ((unsigned int)buf[j - 3][i - 3].g + (unsigned int)buf[j + 3][i - 3].g) >> 1;
                b = ((unsigned int)buf[j - 3][i - 3].b + (unsigned int)buf[j + 3][i - 3].b) >> 1;
                r = buf[j + 3][i - 3].r;
                g = buf[j + 3][i - 3].g;
                b = buf[j + 3][i - 3].b;
                if (!flags[j + 1][i - 3]) {
                    buf[j + 1][i - 3].r = r;
                    buf[j + 1][i - 3].g = g;
                    buf[j + 1][i - 3].b = b;
                }
                if (!flags[j + 1][i - 2]) {
                    buf[j + 1][i - 2].r = r;
                    buf[j + 1][i - 2].g = g;
                    buf[j + 1][i - 2].b = b;
                }
            } else { /* else have to calc lower-left quad */
                for (i = x; i < x + 4; i++) {
                    for (j = 3; j < 7; j++) {
                        if (!flags[j][i]) {
                            flags[j][i] = 1;
                            shoot((double)i + 0.5, (double)y + 0.5 + j, color);
                            buf[j][i] = color.getPixelColor();
                        }
                    }
                }
            } /* end of lower-left quad */

            /* finally finish with lower right quad */
            i = x + 3;
            j = (y % 6) + 3; /* middle ray */

            if (comp(buf[j][i].r, buf[j + 3][i + 3].r) && comp(buf[j][i].g, buf[j + 3][i + 3].g) && comp(buf[j][i].b, buf[j + 3][i + 3].b)) { /* close enough so fill */
                if (!flags[j + 1][i + 1]) {
                    buf[j + 1][i + 1].r = buf[j][i].r;
                    buf[j + 1][i + 1].g = buf[j][i].g;
                    buf[j + 1][i + 1].b = buf[j][i].b;
                }
                if (!flags[j + 2][i + 2]) {
                    buf[j + 2][i + 2].r = buf[j + 3][i + 3].r;
                    buf[j + 2][i + 2].g = buf[j + 3][i + 3].g;
                    buf[j + 2][i + 2].b = buf[j + 3][i + 3].b;
                }
                if (!flags[j + 3][i + 2]) {
                    buf[j + 3][i + 2].r = buf[j + 3][i + 3].r;
                    buf[j + 3][i + 2].g = buf[j + 3][i + 3].g;
                    buf[j + 3][i + 2].b = buf[j + 3][i + 3].b;
                }
                if (!flags[j + 2][i - 3]) {
                    buf[j + 2][i + 3].r = buf[j + 3][i + 3].r;
                    buf[j + 2][i + 3].g = buf[j + 3][i + 3].g;
                    buf[j + 2][i + 3].b = buf[j + 3][i + 3].b;
                }
                r = ((unsigned int)buf[j + 3][i - 3].r + (unsigned int)buf[j + 3][i + 3].r) >> 1;
                g = ((unsigned int)buf[j + 3][i - 3].g + (unsigned int)buf[j + 3][i + 3].g) >> 1;
                b = ((unsigned int)buf[j + 3][i - 3].b + (unsigned int)buf[j + 3][i + 3].b) >> 1;
                r = buf[j + 3][i + 3].r;
                g = buf[j + 3][i + 3].g;
                b = buf[j + 3][i + 3].b;
                if (!flags[j + 3][i + 1]) {
                    buf[j + 3][i + 1].r = r;
                    buf[j + 3][i + 1].g = g;
                    buf[j + 3][i + 1].b = b;
                }
                if (!flags[j + 2][i + 1]) {
                    buf[j + 2][i + 1].r = r;
                    buf[j + 2][i + 1].g = g;
                    buf[j + 2][i + 1].b = b;
                }
                r = ((unsigned int)buf[j - 3][i + 3].r + (unsigned int)buf[j + 3][i + 3].r) >> 1;
                g = ((unsigned int)buf[j - 3][i + 3].g + (unsigned int)buf[j + 3][i + 3].g) >> 1;
                b = ((unsigned int)buf[j - 3][i + 3].b + (unsigned int)buf[j + 3][i + 3].b) >> 1;
                r = buf[j + 3][i + 3].r;
                g = buf[j + 3][i + 3].g;
                b = buf[j + 3][i + 3].b;
                if (!flags[j + 1][i + 3]) {
                    buf[j + 1][i + 3].r = r;
                    buf[j + 1][i + 3].g = g;
                    buf[j + 1][i + 3].b = b;
                }
                if (!flags[j + 1][i + 2]) {
                    buf[j + 1][i + 2].r = r;
                    buf[j + 1][i + 2].g = g;
                    buf[j + 1][i + 2].b = b;
                }
            } else { /* else have to calc lower-right quad */
                for (i = x + 3; i < x + 7; i++) {
                    for (j = 3; j < 7; j++) {
                        if (!flags[j][i]) {
                            flags[j][i] = 1;
                            shoot((double)i + 0.5, (double)y + 0.5 + j, color);
                            buf[j][i] = color.getPixelColor();
                        }
                    }
                }
            } /* end of lower-right quad */

        } /* end of x loop */

        /* output scans */
        for (j = 0; j < 6; j++) {
            picFile->writeLine(buf[j]);
        }
        if (tickflag)
            Stats::statistics(y + 6);
    } /* end of y loop (finally!) */
    if (tickflag)
        cerr << endl;
} /* end of Scan2() */

/*
    Scan3 -- Adaptive supersampling with optional jitter.
         Jitter +- 0.125

    unsigned char win[SIDE + 1][SIDE + 1][4]; // r,g,b,flag TCE: Why [5][5][4]
*/
void Screen_3D::scan3(void) {
    Pixel *pixelBuf;
    Color color; // color of current traced ray
    int x, y, i, j;

    unsigned char *buff[4]; //TCE: Why 4

    pixelBuf = new Pixel[x_res]();


    for (x = 0; x < 4; x++) {
        buff[x] = new unsigned char[SIDE * x_res + 1](); // Why SIDE*x_res+1

    }

    for (i = 0; i < SIDE + 1; i++) { // clear win flags
        for (j = 0; j < SIDE + 1; j++) {
            win[i][j][3] = RAW;
        }
    }
    for (i = 0; i < SIDE * x_res + 1; i++) { // clear buff flags
        buff[3][i] = RAW;
    }

    for (y = start_line; y < stop_line; y++) {

        // clear left edge of win for starting a new row
        for (j = 0; j < SIDE + 1; j++) {
            win[0][j][3] = RAW;
        }

        for (x = 0; x < x_res; x++) {

            for (i = 1; i < SIDE + 1; i++)                  // buff to top row of win
                if (win[i][0][3] = buff[3][x * SIDE + i]) { // if cooked :TCE says ?Huh? flag portion? and an assignment during if?
                    win[i][0][0] = buff[0][x * SIDE + i];   // TCE: x*SIDE+i looks like a buff overrun scenario
                    win[i][0][1] = buff[1][x * SIDE + i];   // buff was created with [4][SIDE * x_res + 1]
                    win[i][0][2] = buff[2][x * SIDE + i];   // With operator precedence and all i should have a max value of 1
                }

            for (i = 1; i < SIDE + 1; i++) // clear rest of win
                for (j = 1; j < SIDE + 1; j++)
                    win[i][j][3] = RAW;
            color = 0.0; // TCE: added this
            adapt(0, 0, (double)x, (double)y, color, SIDE);

            pixelBuf[x].r = color.r;
            pixelBuf[x].g = color.g;
            pixelBuf[x].b = color.b;

            for (i = 0; i < SIDE + 1; i++)                     /* bottom row of win to buff */
                if (buff[3][x * SIDE + i] = win[i][SIDE][3]) { /* if cooked */
                    buff[0][x * SIDE + i] = win[i][SIDE][0];
                    buff[1][x * SIDE + i] = win[i][SIDE][1];
                    buff[2][x * SIDE + i] = win[i][SIDE][2];
                }

            for (j = 0; j < SIDE + 1; j++) {          /* right edge of win to left */
                if (win[0][j][3] = win[SIDE][j][3]) { /* if cooked */
                    win[0][j][0] = win[SIDE][j][0];
                    win[0][j][1] = win[SIDE][j][1];
                    win[0][j][2] = win[SIDE][j][2];
                }
            }
        }
        picFile->writeLine(pixelBuf);
        if (tickflag)
            Stats::statistics(y);
    }
    if (tickflag)
        cout << endl;

    delete pixelBuf;
    for (i = 0; i < 4; i++)
        if (buff[i])
            delete buff[i];

} /* end of Scan3() */

#define ARAND() (((rand() / (double)RAND_MAX) / 4.0) - 0.125)

// TODO: TCE any way we can do this iterativly instead of recursivly?
// used in scan3
//    int    i, j;        /* where in win to put results */
//    double    x, y;        /* upper left hand of pixel */
//    Color  color;       /* return pixel color here in 0..255 range */ //TODO: TCE should this be Pixel and not color as color should remain 0.0 to 1.0 values
//    int    step;        /* what level we're at with values (4, 2, 1)*/
void Screen_3D::adapt(int i, int j, double x, double y, Color &color, int step) {
    int k, fuzzed;
    int ave[3], c0[3], c1[3], c2[3], c3[3];

    if (win[i][j][3] == RAW) {
        if (jitter) {
            shoot(x + (double)i / SIDE + ARAND(), y + (double)j / SIDE + ARAND(), color);
        } else {
            shoot(x + (double)i / SIDE, y + (double)j / SIDE, color);
        }

        Pixel p = color.getPixelColor();
        c0[0] = win[i][j][0] = p.r;
        c0[1] = win[i][j][1] = p.g;
        c0[2] = win[i][j][2] = p.b;


        if (fuzzy_ray) {
            win[i][j][3] = COOKED | FUZZY;
        } else {
            win[i][j][3] = COOKED;
        }
    } else {
        c0[0] = win[i][j][0];
        c0[1] = win[i][j][1];
        c0[2] = win[i][j][2];
    }

    if (win[i + step][j][3] == RAW) {
        if (jitter) {
            shoot((double)x + (double)(i + step) / SIDE + ARAND(), (double)y + (double)j / SIDE + ARAND(), color);
        } else {
            shoot((double)x + (double)(i + step) / SIDE, (double)y + (double)j / SIDE, color);
        }

        Pixel p = color.getPixelColor();
        c1[0] = win[i + step][j][0] = p.r;
        c1[1] = win[i + step][j][1] = p.g;
        c1[2] = win[i + step][j][2] = p.b;

        if (fuzzy_ray) {
            win[i + step][j][3] = COOKED | FUZZY;
        } else {
            win[i + step][j][3] = COOKED;
        }
    } else {
        c1[0] = win[i + step][j][0];
        c1[1] = win[i + step][j][1];
        c1[2] = win[i + step][j][2];
    }

    if (win[i][j + step][3] == RAW) {
        if (jitter) {
            shoot((double)x + (double)i / SIDE + ARAND(), (double)y + (double)(j + step) / SIDE + ARAND(), color);
        } else {
            shoot((double)x + (double)i / SIDE, (double)y + (double)(j + step) / SIDE, color);
        }

        Pixel p = color.getPixelColor();
        c2[0] = win[i][j + step][0] = p.r;
        c2[1] = win[i][j + step][1] = p.g;
        c2[2] = win[i][j + step][2] = p.b;
        if (fuzzy_ray) {
            win[i][j + step][3] = COOKED | FUZZY;
        } else {
            win[i][j + step][3] = COOKED;
        }
    } else {
        c2[0] = win[i][j + step][0];
        c2[1] = win[i][j + step][1];
        c2[2] = win[i][j + step][2];
    }

    if (win[i + step][j + step][3] == RAW) {
        if (jitter) {
            shoot((double)x + (double)(i + step) / SIDE + ARAND(), (double)y + (double)(j + step) / SIDE + ARAND(), color);
        } else {
            shoot((double)x + (double)(i + step) / SIDE, (double)y + (double)(j + step) / SIDE, color);
        }

        Pixel p = color.getPixelColor();
        c3[0] = win[i + step][j + step][0] = p.r;
        c3[1] = win[i + step][j + step][1] = p.g;
        c3[2] = win[i + step][j + step][2] = p.b;

        if (fuzzy_ray) {
            win[i + step][j + step][3] = COOKED | FUZZY;
        } else {
            win[i + step][j + step][3] = COOKED;
        }
    } else {
        c3[0] = win[i + step][j + step][0];
        c3[1] = win[i + step][j + step][1];
        c3[2] = win[i + step][j + step][2];
    }

    for (k = 0; k < 3; k++) {
        ave[k] = c0[k] + c1[k] + c2[k] + c3[k];
        ave[k] /= 4;
    }

    fuzzed = win[i]       [j]       [3]
           & win[i + step][j]       [3]
           & win[i]       [j + step][3]
           & win[i + step][j + step][3]
           & FUZZY;

    if (step == 1 || fuzzed
    || bMath::abs(ave[0] - c0[0]) < adapt_dist
    && bMath::abs(ave[1] - c0[1]) < adapt_dist
    && bMath::abs(ave[2] - c0[2]) < adapt_dist
    && bMath::abs(ave[0] - c1[0]) < adapt_dist
    && bMath::abs(ave[1] - c1[1]) < adapt_dist
    && bMath::abs(ave[2] - c1[2]) < adapt_dist
    && bMath::abs(ave[0] - c2[0]) < adapt_dist
    && bMath::abs(ave[1] - c2[1]) < adapt_dist
    && bMath::abs(ave[2] - c2[2]) < adapt_dist
    && bMath::abs(ave[0] - c3[0]) < adapt_dist
    && bMath::abs(ave[1] - c3[1]) < adapt_dist
    && bMath::abs(ave[2] - c3[2]) < adapt_dist) { /* close enough */
        color.r = ave[0];
        color.g = ave[1];
        color.b = ave[2];

        return;
    }

    /* not close, so we have to subdivide */

    step /= 2;
    ave[0] = 0;
    ave[1] = 0;
    ave[2] = 0;

    adapt(i, j, (double)x, (double)y, color, step);
    ave[0] += (unsigned char)(color.r);
    ave[1] += (unsigned char)(color.g);
    ave[2] += (unsigned char)(color.b);

    adapt(i + step, j, (double)x, (double)y, color, step);
    ave[0] += (unsigned char)(color.r);
    ave[1] += (unsigned char)(color.g);
    ave[2] += (unsigned char)(color.b);

    adapt(i, j + step, (double)x, (double)y, color, step);
    ave[0] += (unsigned char)(color.r);
    ave[1] += (unsigned char)(color.g);
    ave[2] += (unsigned char)(color.b);

    adapt(i + step, j + step, (double)x, (double)y, color, step);
    ave[0] += (unsigned char)(color.r);
    ave[1] += (unsigned char)(color.g);
    ave[2] += (unsigned char)(color.b);

    color.r = ave[0] >> 2;
    color.g = ave[1] >> 2;
    color.b = ave[2] >> 2;

    return;
} /* end of Adapt() */

// double    x, y;   /* where on screen to shoot */
// Color &color;  /* color to return from shot */
void Screen_3D::shoot(double x, double y, Color &color) {
    double random;
    Ray ray2; /* ray tweeked for non-pinhole cameras */
    Vec dir;
    int sample;
    double tx, ty, scale, P;

    switch (camera.projection) {
    case P_FLAT:
    case P_NO_PARALLAX:
        VecComb(-frustrumheight * (2.0 * y / (double)y_res - 1.0),
                looking_up,
                frustrumwidth * (2.0 * x / (double)x_res - 1.0),
                leftvec,
                dir);
        VecAdd(dir, viewvec, ray.D);
        VecNormalize(ray.D);
        break;
    case P_FISHEYE:
        tx = (x - x_res / 2.0) / (double)x_res * camera.eye.view_angle_x * 2.0;
        ty = -(y - y_res / 2.0) / (double)y_res * camera.eye.view_angle_y * 2.0;

        VecComb(sin(ty), looking_up, sin(tx), leftvec, dir);
        VecAddS(cos(tx) * cos(ty), viewvec, dir, ray.D);
        VecNormalize(ray.D);
        break;
    case P_ORTHOGRAPHIC:
        VecComb(-(camera.eye.view_angle_y) * (2.0 * y / (double)y_res - 1.0), looking_up, camera.eye.view_angle_x * (2.0 * x / (double)x_res - 1.0), leftvec, dir);
        VecAdd(dir, viewpoint, ray.P);
        break;
    } /* end of projection switch */

    fuzzy_ray = 0;
    if (camera.aperture > 0.0) {
        Color sum_color; /* summed color for DOF effects */

        for (sample = 0; sample < camera.samples; sample++) {
            dir[0] = ray.P[0] + ray.D[0] * camera.focal_length;
            dir[1] = ray.P[1] + ray.D[1] * camera.focal_length;
            dir[2] = ray.P[2] + ray.D[2] * camera.focal_length;
            VecCopy(ray.P, ray2.P);
            random = rnd();
            if (rnd() > 0.5)
                random = -random;
            VecAddS(random, camera.lens_i, ray2.P, ray2.P);
            random = rnd();
            if (rnd() > 0.5)
                random = -random;
            VecAddS(random, camera.lens_j, ray2.P, ray2.P);
            VecSub(dir, ray2.P, ray2.D);
            VecNormalize(ray2.D);

            RayTrace_3D::trace(0, 1.0, &ray2, color, 1.0, NULL);
            if (color.r > 1.0) color.r = 1.0;
            if (color.g > 1.0) color.g = 1.0;
            if (color.b > 1.0) color.b = 1.0;

            sum_color += color;
        }
        double cs = (1.0 / (double)camera.samples);
        color = sum_color * cs;
    } else {
        RayTrace_3D::trace(0, 1.0, &ray, color, 1.0, NULL);
        if (color.r > 1.0) color.r = 1.0;
        if (color.g > 1.0) color.g = 1.0;
        if (color.b > 1.0) color.b = 1.0;
    }
} /* end of shoot */
