#pragma once

#include "struct_defs.hpp"
#include "String.hpp"

class Screen_3D {
    private:
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