#pragma once

#include <cmath>

class Vector_3D {
  protected:
    double v[3];

  public:
    Vector_3D() {
        v[0] = 0.0;
        v[1] = 0.0;
        v[2] = 0.0;
    }
    Vector_3D(const Vector_3D &vec) {
        v[0] = vec.v[0];
        v[1] = vec.v[1];
        v[2] = vec.v[2];
    }
    Vector_3D(const double a, const double b, const double c) {
        v[0] = a;
        v[1] = b;
        v[2] = c;
    }

    double normalize();

    inline Vector_3D negate() {
        v[0] = -v[0];
        v[1] = -v[1];
        v[2] = -v[2];
        return *this;
    }

    inline double sum() { return v[0] + v[1] + v[2]; }

    double &operator[](int i);
    const double &operator[](int i) const;

    Vector_3D &operator=(const double &c);
    Vector_3D &operator=(const int &c);
    Vector_3D &operator=(const Vector_3D &c);
    Vector_3D &operator*=(const Vector_3D &c);
    Vector_3D &operator*=(const double &c);
    Vector_3D &operator/=(const double &c);
    Vector_3D &operator+=(const Vector_3D &c);
    Vector_3D &operator+=(const int &c);
    Vector_3D &operator-=(const Vector_3D &c);

    friend double dotProduct(const Vector_3D &a, const Vector_3D &b);
    friend const Vector_3D crossProduct(const Vector_3D &a, const Vector_3D &b);

    friend Vector_3D operator*(const Vector_3D &a, const Vector_3D &b);
    friend Vector_3D operator*(const double &a, const Vector_3D &b);
    friend Vector_3D operator*(const Vector_3D &a, const double &b);
    friend Vector_3D operator+(const Vector_3D &a, const Vector_3D &b);
    friend Vector_3D operator-(const Vector_3D &a, const Vector_3D &b);
    friend Vector_3D operator-(const Vector_3D &a);
};

inline double dotProduct(const Vector_3D &a, const Vector_3D &b) { return (a.v[0] * b.v[0]) + (a.v[1] * b.v[1]) + (a.v[2] * b.v[2]); }

inline const Vector_3D crossProduct(const Vector_3D &a, const Vector_3D &b) {
    Vector_3D c;

    c.v[0] = (a.v[1] * b.v[2]) - (a.v[2] * b.v[1]);
    c.v[1] = (a.v[2] * b.v[0]) - (a.v[0] * b.v[2]);
    c.v[2] = (a.v[0] * b.v[1]) - (a.v[1] * b.v[0]);

    return c;
}

inline Vector_3D operator*(const Vector_3D &a, const Vector_3D &b) {
    Vector_3D c = a;
    c *= b;
    return c;
}
inline Vector_3D operator*(const double &a, const Vector_3D &b) {
    Vector_3D c = b;
    c *= a;
    return c;
}
inline Vector_3D operator*(const Vector_3D &a, const double &b) {
    Vector_3D c = a;
    c *= b;
    return c;
}
inline Vector_3D operator+(const Vector_3D &a, const Vector_3D &b) {
    Vector_3D c = a;
    c += b;
    return c;
}
inline Vector_3D operator-(const Vector_3D &a, const Vector_3D &b) {
    Vector_3D c = a;
    c -= b;
    return c;
}
inline Vector_3D operator-(const Vector_3D &a) {
    Vector_3D b = a;
    b.v[0] = -a.v[0];
    b.v[1] = -a.v[1];
    b.v[2] = -a.v[2];
    return b;
}

typedef Vector_3D Vec;

inline void MakeVector(double a, double b, double c, Vector_3D &v) { v = Vector_3D(a, b, c); }
inline double VecDot(Vector_3D &a, Vector_3D &b) { return dotProduct(a, b); }
inline void VecCross(Vector_3D &a, Vector_3D &b, Vector_3D &c) { c = crossProduct(a, b); }
inline double VecLen(Vector_3D &a) { return sqrt(dotProduct(a, a)); }
inline void VecSub(const Vector_3D &a, const Vector_3D &b, Vector_3D &c) { c = a - b; }
inline void VecAddS(double a, const Vector_3D &b, const Vector_3D &c, Vector_3D &d) { d = a * b + c; }
inline void VecCopy(const Vector_3D &a, Vector_3D &b) { b = a; }
inline void VecZero(Vector_3D &a) { a = 0; }
inline void VecS(double a, const Vector_3D &b, Vector_3D &c) { c = a * b; }
inline void VecAdd(const Vector_3D &a, const Vector_3D &b, Vector_3D &c) { c = a + b; }
inline void VecNegate(Vector_3D &a) { a.negate(); }
inline void VecComb(double A, const Vector_3D &a, double B, const Vector_3D &b, Vector_3D &c) { c = (A * a) + (B * b); }
inline double VecNormalize(Vector_3D &vec) { return vec.normalize(); }
