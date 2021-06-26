#include "Color.hpp"

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
