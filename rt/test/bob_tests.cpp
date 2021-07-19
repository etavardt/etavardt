#include <gtest/gtest.h>

#include "Bob.hpp"

//namespace {
TEST(BobTest, singletonCheck) {
    EXPECT_NE(App::app, nullptr); // Bob should already be created
    Bob *bobApp = &Bob::getApp();
    EXPECT_EQ(bobApp, App::app); // Testing of getApp
    Bob *bobApp2 = &Bob::getApp();
    EXPECT_EQ(bobApp2, bobApp); // Retesting of getApp
}

//}
