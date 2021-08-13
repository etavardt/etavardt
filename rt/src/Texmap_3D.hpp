#pragma once
#include "defs.hpp"
#include "Vector_3D.hpp"
#include "String.hpp"
#include "Point_3D.hpp"
#include "Color.hpp"

class Texmap_3D {
    public:
    Texmap_3D() {}
    Texmap_3D(const Texmap_3D &t)
    : position(t.position), /* upper left hand corner of image */
      normal(t.normal),     /* same as projection direction */
      across(t.across),     /* across top of image to upper right */
      down(t.down),         /* down to lower left */
      scale(t.scale),       /* defaults to 1.0, #units across full image */
      xres(t.xres),         /* image size */
      yres(t.yres),         /* image size */
      red(t.red),
      grn(t.grn),
      blu(t.blu) {}
    ~Texmap_3D(){}

    void tex_read_img(const String &filename);
    void get_map_entry(double x, double y, Color &color);
    void tex_project(const Point &P, double *x, double *y);
    void tile(double *x, double *y);

    Vec position,       /* upper left hand corner of image */
        normal,         /* same as projection direction */
        across,         /* across top of image to upper right */
        down;           /* down to lower left */

    double scale = 0.0; /* defaults to 1.0, #units across full image */
    int xres = 0,
        yres = 0;       /* image size */
    unsigned char
        **red = nullptr,
        **grn = nullptr,
        **blu = nullptr;
};

typedef Texmap_3D Texmap;
