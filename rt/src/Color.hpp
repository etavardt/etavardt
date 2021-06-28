#pragma once
#include "defs.hpp"

/*
    Color Normalized 0.0-1.0
    r - Red   component 0.0-1.0
    g - Green component 0.0-1.0
    b - Blue  component 0.0-1.0
*/
class Color {
public:
    double r;
    double g;
    double b;

    Color():r(0.0),g(0.0),b(0.0){};
    ~Color(){};
    
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