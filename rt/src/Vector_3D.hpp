#pragma once

//#include "defs.hpp"
#include <array>
#include <cmath>
//#include <algorithm>

//typedef std::array<double, 4> Array_4;
typedef std::array<double, 3> Array_3;

//typedef double Array_3[3];
/*----------------------------------------------------------------------*/
class Vector_3D {
    public:
    double v[3];

    // Array_3 v;
    // double &v0 = v[0];
    // double &v1 = v[1];
    // double &v2 = v[2];

    //  double v[4];//, &x=v[0], &y=v[1], &z=v[2], &t=v[3];

    Vector_3D() { v[0] = 0.0; v[1] = 0.0; v[2] = 0.0; }
    Vector_3D(const Vector_3D &vec) { v[0] = vec.v[0]; v[1] = vec.v[1]; v[2] = vec.v[2]; }
//    Vector_3D(const Array_3 &a) { v = a; }
    Vector_3D(const double a, const double b, const double c) { v[0] = a; v[1] = b; v[2] = c; }
    // Vector_3D() { v = { 0.0, 0.0, 0.0, 0.0 }; }
    // Vector_3D(const Array_4 &a) { v = a; }
    // Vector_3D(const double a, const double b, const double c, const double d) { v = { a, b, c, d }; }
    // Vector_3D(double _x, double _y, double _z) { x = _x; y = _y; z = _z; t = 0.0; }
    // Vector_3D(double _x, double _y, double _z, double _t) { x = _x; y = _y; z = _z; t = _t; }

    double normalize();

    inline Vector_3D negate() {
        v[0] = -v[0];
        v[1] = -v[1];
        v[2] = -v[2];
        return *this;
    }

    inline double dotProduct(const Vector_3D &a) {
        return (v[0] * a.v[0]) + (v[1] * a.v[1]) + (v[2] * a.v[2]);
    }
    inline const Vector_3D crossProduct(const Vector_3D &_b) {
        Vector_3D t = *this;
        double a[3] = {v[0], v[1], v[2]};
        double b[3] = {_b.v[0], _b.v[1], _b.v[2]};
        double c[3] = {0,0,0};
        // Array_3 a = {v[0], v[1], v[2]};
        // Array_3 b; b = _b.v;
        // Array_3 c;
        c[0] = (a[1] * b[2]) - (a[2] * b[1]);
        c[1] = (a[2] * b[0]) - (a[0] * b[2]);
        c[2] = (a[0] * b[1]) - (a[1] * b[0]);

        // t.v = c;
        // t.v = {c[0],c[1],c[2]};
        t.v[0] = c[0];
        t.v[1] = c[1];
        t.v[2] = c[2];
        return t;
    }

    inline double sum() {
        return v[0] + v[1] + v[2];
    }

    double &operator[](int i);
    const double &operator[](int) const;


    Vector_3D &operator=(const double &c);
    Vector_3D &operator=(const int &c);
    Vector_3D &operator=(const Vector_3D &c);
    Vector_3D &operator*=(const Vector_3D &c);
    Vector_3D &operator*=(const double &c);
    Vector_3D &operator/=(const double &c);
    Vector_3D &operator+=(const Vector_3D &c);
    Vector_3D &operator+=(const int &c);
    Vector_3D &operator-=(const Vector_3D &c);

    friend Vector_3D operator*(const Vector_3D &a, const Vector_3D &b);
    friend Vector_3D operator*(const double &a, const Vector_3D &b);
    friend Vector_3D operator*(const Vector_3D &a, const double &b);
    friend Vector_3D operator+(const Vector_3D &a, const Vector_3D &b);
    friend Vector_3D operator-(const Vector_3D &a, const Vector_3D &b);
    friend Vector_3D operator-(const Vector_3D &a);

    // friend void MakeVector(double a, double b, double c, Vector_3D &v);
    // friend double VecDot(Vector_3D &a, Vector_3D &b);
    // friend void VecCross(Vector_3D &a, Vector_3D &b, Vector_3D &c);
    // friend double VecLen(Vector_3D &a);
    // friend void VecSub(const Vector_3D &a, const Vector_3D &b, Vector_3D &c);
    // friend void VecAddS(double a, const Vector_3D &b, const Vector_3D &c, Vector_3D &d);
    // friend void VecCopy(const Vector_3D &a, Vector_3D &b);
    // friend void VecZero(Vector_3D &a);
    // friend void VecS(double a, const Vector_3D &b, Vector_3D &c);
    // friend void VecAdd(const Vector_3D &a, const Vector_3D &b, Vector_3D &c);
    // friend void VecNegate(Vector_3D &a);
    // friend void VecComb(double A, const Vector_3D &a, double B, const Vector_3D &b, Vector_3D &c);
    // friend double VecNormalize (Vector_3D &vec);

};

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
//typedef Vec Point;

inline void MakeVector(double a, double b, double c, Vector_3D &v) { v = Vector_3D( a, b, c ); }
inline double VecDot(Vector_3D &a, Vector_3D &b) { return a.dotProduct(b); }
inline void VecCross(Vector_3D &a, Vector_3D &b, Vector_3D &c) { c = a.crossProduct(b); }
inline double VecLen(Vector_3D &a) { return sqrt(VecDot(a, a)); }
inline void VecSub(const Vector_3D &a, const Vector_3D &b, Vector_3D &c) { c = a - b; }
inline void VecAddS(double a, const Vector_3D &b, const Vector_3D &c, Vector_3D &d) { d = a * b + c; }
inline void VecCopy(const Vector_3D &a, Vector_3D &b) { b = a; }
inline void VecZero(Vector_3D &a) { a = 0; }
inline void VecS(double a, const Vector_3D &b, Vector_3D &c) { c = a * b; }
inline void VecAdd(const Vector_3D &a, const Vector_3D &b, Vector_3D &c) { c = a + b; }
inline void VecNegate(Vector_3D &a) { a.negate(); }
inline void VecComb(double A, const Vector_3D &a, double B, const Vector_3D &b, Vector_3D &c) { c = (A * a) + (B * b); }
inline double VecNormalize (Vector_3D &vec) { return vec.normalize(); }

/*
inline void MakeVector(x, y, z, v) (v)[0] = (x), (v)[1] = (y), (v)[2] = (z)
#define VecNegate(a)    \
    (a)[0] = (-(a)[0]); \
    (a)[1] = (-(a)[1]); \
    (a)[2] = (-(a)[2]);
#define VecDot(a, b) ((a)[0] * (b)[0] + (a)[1] * (b)[1] + (a)[2] * (b)[2])
#define VecLen(a) (sqrt(VecDot(a, a)))
#define VecCopy(a, b) \
    (b)[0] = (a)[0];  \
    (b)[1] = (a)[1];  \
    (b)[2] = (a)[2];
#define VecAdd(a, b, c)       \
    (c)[0] = (a)[0] + (b)[0]; \
    (c)[1] = (a)[1] + (b)[1]; \
    (c)[2] = (a)[2] + (b)[2]
#define VecSub(a, b, c)       \
    (c)[0] = (a)[0] - (b)[0]; \
    (c)[1] = (a)[1] - (b)[1]; \
    (c)[2] = (a)[2] - (b)[2]
#define VecComb(A, a, B, b, c)            \
    (c)[0] = (A) * (a)[0] + (B) * (b)[0]; \
    (c)[1] = (A) * (a)[1] + (B) * (b)[1]; \
    (c)[2] = (A) * (a)[2] + (B) * (b)[2]
#define VecS(A, a, b)      \
    (b)[0] = (A) * (a)[0]; \
    (b)[1] = (A) * (a)[1]; \
    (b)[2] = (A) * (a)[2]
#define VecAddS(A, a, b, c)         \
    (c)[0] = (A) * (a)[0] + (b)[0]; \
    (c)[1] = (A) * (a)[1] + (b)[1]; \
    (c)[2] = (A) * (a)[2] + (b)[2]
#define VecMul(a, b, c)       \
    (c)[0] = (a)[0] * (b)[0]; \
    (c)[1] = (a)[1] * (b)[1]; \
    (c)[2] = (a)[2] * (b)[2]
#define VecCross(a, b, c)                       \
    (c)[0] = (a)[1] * (b)[2] - (a)[2] * (b)[1]; \
    (c)[1] = (a)[2] * (b)[0] - (a)[0] * (b)[2]; \
    (c)[2] = (a)[0] * (b)[1] - (a)[1] * (b)[0]
#define VecZero(v) \
    (v)[0] = 0.0;  \
    (v)[1] = 0.0;  \
    v[2] = 0.0
#define VecPrint(msg, v) fprintf(stderr, "%s: %g %g %g\n", msg, \
                                 (v)[0], (v)[1], (v)[2])

 */
