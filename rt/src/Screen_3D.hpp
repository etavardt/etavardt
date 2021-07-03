#pragma once

#include "struct_defs.hpp"
#include "String.hpp"
#include "Ray_3D.hpp"
#include "pic.hpp"

#define SIDE (4)

#define RAW (0)
#define COOKED (1)
#define FUZZY (2)

class Screen_3D {
    private:
    Flt frustrumwidth, frustrumheight;
    Point viewvec, leftvec, looking_up, viewpoint;
    Ray ray; /* normal, untweeked ray */
    int x_res, y_res;
    Pic *pic;

    unsigned char win[SIDE + 1][SIDE + 1][4]; /* r,g,b,flag */

    void scrInit(Viewpoint *view, int xres, int yres, String &picfile);
    void scan0(void);
    void scan1(void);
    void scan2(void);
    void scan3(void);

    void shoot(Flt x, Flt y, Color &color);
    void adapt(int i, int j, Flt x, Flt y, Color &color, int step);
    int comp(unsigned int a, unsigned int b);

    public:
    Screen_3D(){}
    ~Screen_3D(){}

    void screen(Viewpoint *view, String &picfile, int xres, int yres);

};