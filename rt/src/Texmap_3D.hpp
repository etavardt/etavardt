#pragma once
#include "defs.hpp"

class Texmap {
    public:
    Texmap(){}
    ~Texmap(){}

    Vec position,       /* upper left hand corner of image */
        normal,         /* same as projection direction */
        across,         /* across top of image to upper right */
        down;           /* down to lower left */
    Flt scale;          /* defaults to 1.0, #units across full image */
    int xres, yres;     /* image size */
    unsigned char
        **red,
        **grn,
        **blu;
};
