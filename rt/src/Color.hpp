#pragma once
#include "Vector_3D.hpp"
// #include "defs.hpp"

struct t_pixel {
    unsigned char r, g, b, q;
};

typedef t_pixel Pixel;

/*
    Color Normalized 0.0-1.0
    r - Red   component 0.0-1.0
    g - Green component 0.0-1.0
    b - Blue  component 0.0-1.0
*/
class Color {
// class Color : public Vector_3D {
  public:
    double r;
    double g;
    double b;

    // double &r = v[0];
    // double &g = v[1];
    // double &b = v[2];

    Color() { r = g = b = 0.0; }
    Color(const Color &c) : r(c.r), g(c.g), b(c.b) {}
    ~Color(){};

    Pixel getPixelColor();

    Color &operator=(const double &c);
    Color &operator=(const int &c);
    Color &operator=(const Vec &c);
    Color &operator=(const Color &c);
    Color &operator*=(const Color &c);
    Color &operator*=(const double &c);
    Color &operator+=(const Color &c);
    Color &operator+=(const int &c);

    friend Color operator*(const Color &a, const Color &b);
    friend Color operator*(const double &a, const Color &b);
    friend Color operator*(const Color &a, const double &b);
    friend Color operator+(const Color &a, const Color &b);
};

inline Color operator*(const Color &a, const Color &b) {
    Color c = a;
    c *= b;
    return c;
}
inline Color operator*(const double &a, const Color &b) {
    Color c = b;
    c *= a;
    return c;
}
inline Color operator*(const Color &a, const double &b) {
    Color c = a;
    c *= b;
    return c;
}
inline Color operator+(const Color &a, const Color &b) {
    Color c = a;
    c += b;
    return c;
}
