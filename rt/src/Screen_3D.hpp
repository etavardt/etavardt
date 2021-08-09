#pragma once

#include "String.hpp"
#include "Ray_3D.hpp"
#include "Color.hpp"
#include "PicFile_3D.hpp"
//#include "Camera_3D.hpp"

class Camera_3D; // avoid circular issue
//class PicFile_3D;

#define SIDE (4)

#define RAW (0)
#define COOKED (1)
#define FUZZY (2)

class Screen_3D {
    private:
    int x_res = 0;
    int y_res = 0;
    double frustrumwidth = 0;
    double frustrumheight = 0;
    Camera_3D &camera;

    PicFile_3D picFile;
    unsigned char win[SIDE + 1][SIDE + 1][4] {{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0}}; /* r,g,b,flag */

    Point viewvec, leftvec, looking_up, viewpoint;
    Ray ray; /* normal, untweeked ray */

    Screen_3D();
    Screen_3D(const Screen_3D &s)
    : x_res(s.x_res), y_res(s.y_res),
      frustrumwidth(s.frustrumwidth),
      frustrumheight(s.frustrumheight),
      camera(s.camera), //picFile(s.picFile),
      start_line(s.start_line), stop_line(s.stop_line) {}

    void scrInit(int xres, int yres, const String &picFileName);
    void scan0(void);
    void scan1(void);
    void scan2(void);
    void scan3(void);

    void shoot(double x, double y, Color &color);
    void adapt(int i, int j, double x, double y, Color &color, int step);
    int comp(unsigned int a, unsigned int b) const;

    inline void shootIfFlagged(int y, Color &color, int i, int j, int *flags[7], Pixel *buf[7]) {
        if (!flags[j][i]) {
            flags[j][i] = 1;
            shoot((double)i + 0.5, (double)y + 0.5, color);
            buf[j][i] = color.getPixelColor();
        }
    }
    inline void fillInBufPosIfFlagged(int i1, int j1, int i2, int j2, int *flags[7], Pixel *buf[7]) {
        if (!flags[j1][i1]) {
            buf[j1][i1].r = buf[j2][i2].r;
            buf[j1][i1].g = buf[j2][i2].g;
            buf[j1][i1].b = buf[j2][i2].b;
        }
    }
    inline void fillInBufPosIfFlagged(int i, int j, const Pixel &p, int *flags[7], Pixel *buf[7]) {
        if (!flags[j][i]) {
            buf[j][i].r = p.r;
            buf[j][i].g = p.g;
            buf[j][i].b = p.b;
        }
    }

    public:
    int start_line = 0;
    int stop_line = 0;

//    explicit Screen_3D(Camera_3D &cam, PicFile_3D &pic);
    explicit Screen_3D(Camera_3D &cam);
    ~Screen_3D() = default;

    void render(const String &picfile, int xres, int yres);

};
