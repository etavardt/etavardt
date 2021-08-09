#include <gtest/gtest.h>

#include "Vector_3D.hpp"

TEST(VectorTest, BasicEqualityCheck) {
    Vector_3D a(1.0, 2.0, 3.0);
    Vector_3D b = Vector_3D{1.0, 2.0, 3.0};
    Vector_3D c = a;

    EXPECT_EQ(a[0], 1.0);
    EXPECT_EQ(a[1], 2.0);
    EXPECT_EQ(a[2], 3.0);
    EXPECT_NE(a[0], a[2]);

    EXPECT_EQ(a[0], b[0]);
    EXPECT_EQ(a[1], b[1]);
    EXPECT_EQ(a[2], c[2]);

    EXPECT_EQ(a[0], c[0]);
    EXPECT_EQ(a[1], c[1]);
    EXPECT_EQ(a[2], c[2]);

    a = Vector_3D(1.0, 1.0, 1.0);

    EXPECT_EQ(c[0], 1.0);
    EXPECT_EQ(c[1], 2.0);
    EXPECT_EQ(c[2], 3.0);
}

TEST(VectorTest, BasicNotEqualityCheck) {
    Vector_3D a(1.0, 2.0, 3.0);
    Vector_3D b(a);

    EXPECT_EQ(a[0], b[0]);
    EXPECT_EQ(a[1], b[1]);
    EXPECT_EQ(a[2], b[2]);

    b[1] = 55;

    EXPECT_EQ(b[0], 1.0);
    EXPECT_EQ(b[1], 55);
    EXPECT_NE(b[2], -993.0);
    EXPECT_NE(a[1], 55);
    EXPECT_EQ(a[1], 2.0);
}

TEST(VectorTest, SumOfSquaresOfDifferences) {
    Vector_3D ap = { 2,3,4 };
    Vector_3D P = { 5,6,7 };
    double dist1 = ((ap-P)*(ap-P)).sum(); // TCE: sum of squares of differences? Hmm?
    double dist2 = (ap[0] - P[0]) * (ap[0] - P[0]) + (ap[1] - P[1]) * (ap[1] - P[1]) + (ap[2] - P[2]) * (ap[2] - P[2]); // TCE: sum of squares of differences? Hmm?

    EXPECT_EQ(dist1, dist2);
}

TEST(VectorTest, VecDotVsVecLenVsMath) {
    Vector_3D V = { 2,3,4 };

    double dist1 = V[0] * V[0] + V[1] * V[1] + V[2] * V[2];
    double dist2 = VecDot(V, V);
    double dist3 = VecLen(V);

    EXPECT_EQ(dist1, dist2);
    EXPECT_NE(dist1, dist3);
    EXPECT_NE(dist3, dist2);
}
