#include "Texmap_3D.hpp"

#include <cstdio>
#include <cstdlib>

#include "Bob.hpp"
#include "String.hpp"
#include "Exception.hpp"
#include "Stats.hpp"
#include "proto.hpp"

Texmap_3D::Texmap_3D(const Texmap_3D &t) {
    position = t.position; /* upper left hand corner of image */
    normal   = t.normal;   /* same as projection direction */
    across   = t.across;   /* across top of image to upper right */
    down     = t.down;     /* down to lower left */
    scale    = t.scale;    /* defaults to 1.0, #units across full image */
    xres     = t.xres;     /* image size */
    yres     = t.yres;     /* image size */
    red      = t.red;
    grn      = t.grn;
    blu      = t.blu;
}


/*
    tex_read_img() -- Read a .img file into a texture map structure
*/
void Texmap_3D::tex_read_img(const String &filename, Texmap &tm) {
    FILE *fp;
    int w, h, /* width and height */
        i, j, cnt, red, grn, blu;

    fp = env_fopen(filename, "rb");
    if (!fp) {
        cerr << "Error opening file " << filename << " for texture mapping." << endl;
        throw Exception("Thrown from tex_read_img");
    }

    /* get width and height from header */
    w = fgetc(fp) << 8;
    w += fgetc(fp);
    h = fgetc(fp) << 8;
    h += fgetc(fp);

    /* waste other stuff */
    fgetc(fp);
    fgetc(fp);
    fgetc(fp);
    fgetc(fp);
    fgetc(fp);
    fgetc(fp);

    /* allocate memory for image in RAM */
    typedef unsigned char *Lines;
    typedef unsigned char rows;

    tm.red = new Lines[h]();
    Stats::trackMemoryUsage(sizeof(Lines[h]));
    //    tm->red = (unsigned char **)vmalloc(sizeof(unsigned char *) * h);
    Bob::getApp().parser.ptrchk(tm.red, "image texture map");
    tm.grn = new Lines[h]();
    Stats::trackMemoryUsage(sizeof(Lines[h]));
    //    tm->grn = (unsigned char **)vmalloc(sizeof(unsigned char *) * h);
    Bob::getApp().parser.ptrchk(tm.grn, "image texture map");
    tm.blu = new Lines[h]();
    Stats::trackMemoryUsage(sizeof(Lines[h]));
    //    tm->blu = (unsigned char **)vmalloc(sizeof(unsigned char *) * h);
    Bob::getApp().parser.ptrchk(tm.blu, "image texture map");
    for (j = 0; j < h; j++) {
        tm.red[j] = new rows[w]();
        Stats::trackMemoryUsage(sizeof(rows[w]));
        //        tm->red[j] = (unsigned char *)vmalloc(sizeof(unsigned char) * w);
        Bob::getApp().parser.ptrchk(tm.red[j], "image texture map");

        tm.grn[j] = new rows[w]();
        Stats::trackMemoryUsage(sizeof(rows[w]));
        //        tm->grn[j] = (unsigned char *)vmalloc(sizeof(unsigned char) * w);
        Bob::getApp().parser.ptrchk(tm.grn[j], "image texture map");

        tm.blu[j] = new rows[w]();
        Stats::trackMemoryUsage(sizeof(rows[w]));
        //        tm->blu[j] = (unsigned char *)vmalloc(sizeof(unsigned char) * w);
        Bob::getApp().parser.ptrchk(tm.blu[j], "image texture map");
    }

    /* read in the image */
    for (j = 0; j < h; j++) {
        i = 0;
        while (i < w) {
            cnt = fgetc(fp) & 0xff;
            blu = fgetc(fp) & 0xff;
            grn = fgetc(fp) & 0xff;
            red = fgetc(fp) & 0xff;
            while (cnt) {
                tm.red[j][i] = red;
                tm.grn[j][i] = grn;
                tm.blu[j][i] = blu;
                i++;
                cnt--;
            }
        }
    } /* end of loop for each scan line */

    fclose(fp);

    tm.xres = w;
    tm.yres = h;
} /* end of tex_read_img() */
