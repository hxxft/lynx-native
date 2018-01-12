// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/position.h"
#include "gtest/gtest.h"

namespace base {
    TEST(PositionTest, IsEmpty) {
        Position p1;
        Position p2(0,0,0,0);
        Position p3(1, -1, 2, -2);
        Position p4(1, -1, 2, 2);

        EXPECT_TRUE(p1.IsEmpty());
        EXPECT_TRUE(p2.IsEmpty());
        EXPECT_TRUE(p3.IsEmpty());
        EXPECT_FALSE(p4.IsEmpty());
    }

    TEST(PositionTest, Update) {
        Position p1(1, -1, 2, -2);
        Vector2D vec(-1, 3);
        p1.Update(vec);

        EXPECT_EQ(0, p1.left_);
        EXPECT_EQ(2, p1.top_);
        EXPECT_EQ(1, p1.right_);
        EXPECT_EQ(1, p1.bottom_);
    }

    TEST(PositionTest, Reset) {
        Position p1(1, -1, 2, -2);
        Position p2(-1, 2, 2, 3);

        p1.Reset(1, 3, 2, -1);
        EXPECT_EQ(1, p1.left_);
        EXPECT_EQ(3, p1.top_);
        EXPECT_EQ(2, p1.right_);
        EXPECT_EQ(-1, p1.bottom_);

        p1.Reset(p2);
        EXPECT_EQ(-1, p1.left_);
        EXPECT_EQ(2, p1.top_);
        EXPECT_EQ(2, p1.right_);
        EXPECT_EQ(3, p1.bottom_);
    }

    TEST(PositionTest, GetWidth) {
        Position p1(1, -1, -2, -2);
        Position p2(1, -1, 2, 2);
        EXPECT_EQ(0, p1.GetWidth());
        EXPECT_EQ(1, p2.GetWidth());
    }

    TEST(PositionTest, GetHeight) {
        Position p1(1, -1, 2, -2);
        Position p2(1, -1, 2, 2);
        EXPECT_EQ(0, p1.GetHeight());
        EXPECT_EQ(3, p2.GetHeight());
    }
}  // namespace base
