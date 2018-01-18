// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/string/string_number_convert.h"

#include "gtest/gtest.h"

namespace base{

TEST(StringNumberConvertTest, StringToDoubleTest) {
  
  double num1 = 0;
  EXPECT_TRUE(StringToDouble("123.456", num1));
  EXPECT_EQ(123.456, num1);

  EXPECT_FALSE(StringToDouble("12c.456", num1));
  EXPECT_EQ(123.456, num1);
}

}