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
void Texmap_3D::tex_read_img(const String &filename) {
    FILE *fp;
    int _w = 0, _h = 0; /* width and height */

    fp = env_fopen(filename, "rb");
    if (!fp) {
        cerr << "Error opening file " << filename << " for texture mapping." << endl;
        throw Exception("Thrown from tex_read_img");
    }

    /* get width and height from header */
    _w = fgetc(fp) << 8;
    _w += fgetc(fp);
    _h = fgetc(fp) << 8;
    _h += fgetc(fp);

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

    red = new Lines[_h]();
    Stats::trackMemoryUsage(sizeof(Lines)*_h);
    //    tm->red = (unsigned char **)vmalloc(sizeof(unsigned char *) * h);

    grn = new Lines[_h]();
    Stats::trackMemoryUsage(sizeof(Lines)*_h);
    //    tm->grn = (unsigned char **)vmalloc(sizeof(unsigned char *) * h);

    blu = new Lines[_h]();
    Stats::trackMemoryUsage(sizeof(Lines)*_h);
    //    tm->blu = (unsigned char **)vmalloc(sizeof(unsigned char *) * h);

    for (int j = 0; j < _h; j++) {
        red[j] = new rows[_w]();
        Stats::trackMemoryUsage(sizeof(rows)*_w);
        //        tm->red[j] = (unsigned char *)vmalloc(sizeof(unsigned char) * w);


        grn[j] = new rows[_w]();
        Stats::trackMemoryUsage(sizeof(rows)*_w);
        //        tm->grn[j] = (unsigned char *)vmalloc(sizeof(unsigned char) * w);


        blu[j] = new rows[_w]();
        Stats::trackMemoryUsage(sizeof(rows)*_w);
        //        tm->blu[j] = (unsigned char *)vmalloc(sizeof(unsigned char) * w);

    }

    /* read in the image */
    for (int j = 0; j < _h; j++) {
        int i = 0;
        while (i < _w) {
            int _cnt = fgetc(fp) & 0xff;
            int _blu = fgetc(fp) & 0xff;
            int _grn = fgetc(fp) & 0xff;
            int _red = fgetc(fp) & 0xff;
            while (_cnt) {
                red[j][i] = _red;
                grn[j][i] = _grn;
                blu[j][i] = _blu;
                i++;
                _cnt--;
            }
        }
    } /* end of loop for each scan line */

    fclose(fp);

    xres = _w;
    yres = _h;
} /* end of tex_read_img() */

/*
    get_map_entry() -- Given a texture structure and the indices
        into the map this function fills in the color at that
        point.  Note that indices are actually backwards.
*/
//TODO: TCE: Belongs in Texmap_3D, move it there once color is fixed again the output files
void Texmap_3D::get_map_entry(double x, double y, Color &color) {
    int i = 0, j = 0;

    /* get integer indices */
    i = x * (xres);
    j = y * (xres);

    color.r = red[j][i] / 255.0;
    color.g = grn[j][i] / 255.0;
    color.b = blu[j][i] / 255.0;

} /* end of get_map_entry() */

/*
    tex_project() -- For the texture defined by surf.tm_???
        project the point P onto the image plane and return
        the indices for the image
*/
//TODO: TCE: Belongs in Texmap_3D, move it there once color is fixed again the output files
void Texmap_3D::tex_project(const Point &P, double *x, double *y) {
    Point PP, /* point projected onto plane of image */
        V;
    double dot = 0.0;
//    Texmap tm(_tm);// = _tm;
    /* project intersection point onto image plane */
    VecSub(P, position, V);
    dot = VecDot(normal, V);
    VecAddS(-dot, normal, P, PP);

    /* calc offsets in across and down directions */
    VecSub(PP, position, V);
    dot = VecDot(across, V);
    *x = dot / scale;
    dot = VecDot(down, V);
    // *y = dot * (double)tm.yres / tm.xres / tm.scale;
    // *y = dot / tm.scale;
    *y = dot / scale;

} /* end of tex_project() */

/*
    tile() -- Take the raw indices and based on the tile pattern return
        the indices that are within the image bounds.
*/
void Texmap_3D::tile(double *x, double *y) {
    *x = fmod(*x, 1.0);
    if (*x < 0.0) {
        *x += 1.0;
    }
    *y = fmod(*y, (double)yres / (double)xres);
    if (*y < 0.0) {
        *y += (double)yres / (double)xres;
    }
} /* end of tile() */
