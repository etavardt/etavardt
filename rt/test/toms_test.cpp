#include <gtest/gtest.h>

#include "Vector_3D.hpp"

namespace {
TEST(TomsTest, BasicEqualityCheck) {
    Vector_3D a = Vector_3D{1.0, 2.0, 3.0};
    Vector_3D b = a;

    EXPECT_EQ(a[0], b[0]);
    EXPECT_EQ(a[1], b[1]);
    EXPECT_EQ(a[2], b[2]);

    a = Vector_3D{1.0, 1.0, 1.0};

    EXPECT_EQ(b[0], 1.0);
    EXPECT_EQ(b[1], 2.0);
    EXPECT_EQ(b[2], 3.0);
}

TEST(TomsTest, BasicNotEqualityCheck) {
    Vector_3D a = Vector_3D{1.0, 2.0, 3.0};
    Vector_3D b = a;

    EXPECT_EQ(a[0], b[0]);
    EXPECT_EQ(a[1], b[1]);
    EXPECT_EQ(a[2], b[2]);

    a = Vector_3D{1.0, 1.0, 1.0};

    EXPECT_EQ(b[0], 1.0);
    EXPECT_EQ(b[1], 2.0);
    EXPECT_NE(b[2], -993.0);
}
}
