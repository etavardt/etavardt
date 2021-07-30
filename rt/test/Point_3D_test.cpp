#include <gtest/gtest.h>

#include "Point_3D.hpp"

TEST(PointTest, BasicEqualityCheck) {
    Point_3D a(1.0, 2.0, 3.0);
    Point_3D b = Point_3D{1.0, 2.0, 3.0};
    Point_3D c = a;

    EXPECT_EQ(a.x, 1.0);
    EXPECT_EQ(a.y, 2.0);
    EXPECT_EQ(a.z, 3.0);
    EXPECT_EQ(a.x, a[0]);
    EXPECT_EQ(a.y, a[1]);
    EXPECT_EQ(a.z, a[2]);
    EXPECT_NE(a.x, a[2]);

    EXPECT_EQ(a[0], b[0]);
    EXPECT_EQ(a[1], b[1]);
    EXPECT_EQ(a[2], c[2]);

    EXPECT_EQ(a[0], c[0]);
    EXPECT_EQ(a[1], c[1]);
    EXPECT_EQ(a[2], c[2]);

    a = Point_3D(1.0, 1.0, 1.0);

    EXPECT_EQ(c[0], 1.0);
    EXPECT_EQ(c[1], 2.0);
    EXPECT_EQ(c[2], 3.0);
}

TEST(PointTest, BasicNotEqualityCheck) {
    Vector_3D a(1.0, 2.0, 3.0);
    Point_3D b(a);

    EXPECT_EQ(a[0], b[0]);
    EXPECT_EQ(a[1], b[1]);
    EXPECT_EQ(a[2], b[2]);
    EXPECT_EQ(a[0], b.x);
    EXPECT_EQ(a[1], b.y);
    EXPECT_EQ(a[2], b.z);

    b.y = 55;

    EXPECT_EQ(b[0], 1.0);
    EXPECT_EQ(b[1], 55);
    EXPECT_NE(b[2], -993.0);
    EXPECT_NE(a[1], 55);
    EXPECT_EQ(a[1], 2.0);
}
