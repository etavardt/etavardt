#include <gtest/gtest.h>

#include "TransformMatrix_3D.hpp"

TEST(TransformMatrix_3D, BasicEqualityCheck) {
    TransformMatrix_3D *a = new TransformMatrix_3D;
    TransformMatrix_3D *b = a;

    EXPECT_EQ(a, b);

    a = new TransformMatrix_3D();

    EXPECT_NE(a, b);
    delete a;
    delete b;
}
