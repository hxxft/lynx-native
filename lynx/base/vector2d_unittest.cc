// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/vector2d.h"
#include "gtest/gtest.h"

namespace base {
    TEST(Vector2DTest, Reset) {
        Vector2D v1(4, -1);

        v1.Reset();
        EXPECT_EQ(0, v1.x());
        EXPECT_EQ(0, v1.y());
    }

    TEST(Vector2DTest, Offset) {
        Vector2D v1(4, -1);

        v1.Offset(3, 5);
        EXPECT_EQ(7, v1.x());
        EXPECT_EQ(4, v1.y());
    }
}  // namespace base
