#include "Color.hpp"
#include "Vector_3D.hpp"

Pixel Color::getPixelColor() {
    Pixel pixel;
    pixel.r = (unsigned char)(255.0 * r);
    pixel.g = (unsigned char)(255.0 * g);
    pixel.b = (unsigned char)(255.0 * b);
    pixel.q = 0;
    return pixel;
}

Color &Color::operator=(const double &c) {
    r = c;
    g = c;
    b = c;
    return *this;
}

Color &Color::operator=(const int &c) {
    r = c;
    g = c;
    b = c;
    return *this;
}

Color &Color::operator=(const Vec &c) {
    r = c[0];
    g = c[1];
    b = c[2];
    return *this;
}
Color &Color::operator=(const Color &c) {
    r = c.r;
    g = c.g;
    b = c.b;
    return *this;
}

Color &Color::operator*=(const Color &c) {
    r *= c.r;
    g *= c.g;
    b *= c.b;
    return *this;
}

Color &Color::operator*=(const double &c) {
    r *= c;
    g *= c;
    b *= c;
    return *this;
}

Color &Color::operator+=(const Color &c) {
    r += c.r;
    g += c.g;
    b += c.b;
    return *this;
}

Color &Color::operator+=(const int &c) {
    r += c;
    g += c;
    b += c;
    return *this;
}
