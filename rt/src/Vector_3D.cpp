#include "Vector_3D.hpp"

#include "Exception.hpp"
#include "BobMath.hpp"
#include "extern.hpp"

double &Vector_3D::operator[](int i) {
    // if (i >= v.size()) {
    if (i >= 3) {
        throw Exception("Array index out of bound, exiting");
    }
    return v[i];
}

const double &Vector_3D::operator[](int i) const {
    // if (i >= v.size()) {
    if (i >= 3) {
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
    // v[3] *= c.v[3];
    return *this;
}

Vector_3D &Vector_3D::operator*=(const double &c) {
    v[0] *= c;
    v[1] *= c;
    v[2] *= c;
    // v[3] *= c;
    return *this;
}
Vector_3D &Vector_3D::operator/=(const double &c) {
    v[0] /= c;
    v[1] /= c;
    v[2] /= c;
    // v[3] *= c;
    return *this;
}

Vector_3D &Vector_3D::operator+=(const Vector_3D &c) {
    v[0] += c.v[0];
    v[1] += c.v[1];
    v[2] += c.v[2];
    // v[3] += c.v[3];
    return *this;
}

Vector_3D &Vector_3D::operator+=(const int &c) {
    v[0] += c;
    v[1] += c;
    v[2] += c;
    // v[3] += c;
    return *this;
}

Vector_3D &Vector_3D::operator-=(const Vector_3D &c) {
    v[0] -= c.v[0];
    v[1] -= c.v[1];
    v[2] -= c.v[2];
    // v[3] += c.v[3];
    return *this;
}

double Vector_3D::normalize() {
    double    len;

//    len = VecDot(*this, *this);
    len = dotProduct(*this);
    if(bMath::abs(len) > rayeps) {
        len = sqrt(len);
        *this /= len;
    } else {
        /* printf("%.4f %.4f %.4f == %.4f bummer\n", vec[0], vec[1], vec[2], len); */
        len = 1.0;
        *this = Vector_3D(1.0, 0.0, 0.0);
    }
    return(len);
}
