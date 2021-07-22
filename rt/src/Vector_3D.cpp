#include "Vector_3D.hpp"

#include "Bob.hpp" // for Bob::rayeps in special func normalize
#include "BobMath.hpp"
#include "Exception.hpp"

double &Vector_3D::operator[](int i) {
    if (i < 0 || i > 2) {
        throw Exception("Array index out of bound, exiting");
    }

    return v[i];
}

const double &Vector_3D::operator[](int i) const {
    if (i < 0 || i > 2) {
        throw Exception("Array index out of bound, exiting");
    }

    return v[i];
}

Vector_3D &Vector_3D::operator=(const double &c) {
    v[0] = c;
    v[1] = c;
    v[2] = c;

    return *this;
}

Vector_3D &Vector_3D::operator=(const int &c) {
    v[0] = c;
    v[1] = c;
    v[2] = c;

    return *this;
}

Vector_3D &Vector_3D::operator=(const Vector_3D &c) {
    v[0] = c.v[0];
    v[1] = c.v[1];
    v[2] = c.v[2];

    return *this;
}

Vector_3D &Vector_3D::operator*=(const Vector_3D &c) {
    v[0] *= c.v[0];
    v[1] *= c.v[1];
    v[2] *= c.v[2];

    return *this;
}

Vector_3D &Vector_3D::operator*=(const double &c) {
    v[0] *= c;
    v[1] *= c;
    v[2] *= c;

    return *this;
}
Vector_3D &Vector_3D::operator/=(const double &c) {
    v[0] /= c;
    v[1] /= c;
    v[2] /= c;

    return *this;
}

Vector_3D &Vector_3D::operator+=(const Vector_3D &c) {
    v[0] += c.v[0];
    v[1] += c.v[1];
    v[2] += c.v[2];

    return *this;
}

Vector_3D &Vector_3D::operator+=(const int &c) {
    v[0] += c;
    v[1] += c;
    v[2] += c;

    return *this;
}

Vector_3D &Vector_3D::operator-=(const Vector_3D &c) {
    v[0] -= c.v[0];
    v[1] -= c.v[1];
    v[2] -= c.v[2];

    return *this;
}

double Vector_3D::normalize() {
    double len;

    len = VecDot(*this, *this);
    if (bMath::abs(len) > Bob::rayeps) {
        len = sqrt(len);
        *this /= len;
    } else {
        len = 1.0;
        *this = Vector_3D(1.0, 0.0, 0.0);
    }

    return (len);
}
