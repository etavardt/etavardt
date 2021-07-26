#pragma once

#include "String.hpp"
#include "Ray_3D.hpp"
#include "Color.hpp"
//#include "Camera_3D.hpp"

class Camera_3D; // avoid circular issue
class PicFile_3D;

#define SIDE (4)

#define RAW (0)
#define COOKED (1)
#define FUZZY (2)

class Screen_3D {
    private:
    Screen_3D();

    Camera_3D &camera;
    double frustrumwidth, frustrumheight;
    Point viewvec, leftvec, looking_up, viewpoint;
    Ray ray; /* normal, untweeked ray */
    int x_res, y_res;
    PicFile_3D *picFile = nullptr;

    unsigned char win[SIDE + 1][SIDE + 1][4]; /* r,g,b,flag */

    void scrInit(int xres, int yres, String &picFileName);
    void scan0(void);
    void scan1(void);
    void scan2(void);
    void scan3(void);

    void shoot(double x, double y, Color &color);
    void adapt(int i, int j, double x, double y, Color &color, int step);
    int comp(unsigned int a, unsigned int b);

    public:
    Screen_3D(Camera_3D &cam);
    ~Screen_3D();

    void screen(String &picfile, int xres, int yres);

};
