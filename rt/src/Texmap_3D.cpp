#include "Texmap_3D.hpp"

#include <cstdio>
#include <cstdlib>

#include "Bob.hpp"
#include "String.hpp"
#include "Exception.hpp"
#include "Stats.hpp"


extern FILE *env_fopen(const String &name, const String &mode); // in file.cpp

/*
    tex_read_img() -- Read a .img file into a texture map structure
*/
void Texmap_3D::tex_read_img(const String &filename, Texmap &tm) {
    FILE *fp;
    int w = 0, h = 0; /* width and height */

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
    Stats::trackMemoryUsage(sizeof(Lines)*h);
    //    tm->red = (unsigned char **)vmalloc(sizeof(unsigned char *) * h);

    tm.grn = new Lines[h]();
    Stats::trackMemoryUsage(sizeof(Lines)*h);
    //    tm->grn = (unsigned char **)vmalloc(sizeof(unsigned char *) * h);

    tm.blu = new Lines[h]();
    Stats::trackMemoryUsage(sizeof(Lines)*h);
    //    tm->blu = (unsigned char **)vmalloc(sizeof(unsigned char *) * h);

    for (int j = 0; j < h; j++) {
        tm.red[j] = new rows[w]();
        Stats::trackMemoryUsage(sizeof(rows)*w);
        //        tm->red[j] = (unsigned char *)vmalloc(sizeof(unsigned char) * w);


        tm.grn[j] = new rows[w]();
        Stats::trackMemoryUsage(sizeof(rows)*w);
        //        tm->grn[j] = (unsigned char *)vmalloc(sizeof(unsigned char) * w);


        tm.blu[j] = new rows[w]();
        Stats::trackMemoryUsage(sizeof(rows)*w);
        //        tm->blu[j] = (unsigned char *)vmalloc(sizeof(unsigned char) * w);

    }

    /* read in the image */
    for (int j = 0; j < h; j++) {
        int i = 0;
        while (i < w) {
            int cnt = fgetc(fp) & 0xff;
            int blu = fgetc(fp) & 0xff;
            int grn = fgetc(fp) & 0xff;
            int red = fgetc(fp) & 0xff;
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
