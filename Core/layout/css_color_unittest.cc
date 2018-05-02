#include "layout/css_color.h"

#include "gtest/gtest.h"

namespace lynx {
TEST(CSSColorTest, ColorParserTest) {
  CSSColor color;
  EXPECT_FALSE(CSSColor::Parse("test", color));
  EXPECT_FALSE(CSSColor::Parse("#test", color));
  EXPECT_FALSE(CSSColor::Parse("#ff0affaa", color));
  EXPECT_FALSE(CSSColor::Parse("rgb(test)", color));
  EXPECT_FALSE(CSSColor::Parse("rgb(1, 2)", color));
  EXPECT_FALSE(CSSColor::Parse("rgb(1, test)", color));
  EXPECT_FALSE(CSSColor::Parse("rgba(1, 123, 1)", color));
  EXPECT_FALSE(CSSColor::Parse("rgba(test, 1, 123, 1)", color));
  EXPECT_FALSE(CSSColor::Parse("rgba(1, test, 123, 1)", color));
  EXPECT_FALSE(CSSColor::Parse("rgba(1, 123, test, 1)", color));
  EXPECT_FALSE(CSSColor::Parse("rgba(256, 1, 123, test)", color));

  EXPECT_TRUE(CSSColor::Parse("red", color));
  EXPECT_EQ(CSSColor(255, 0, 0, 1), color);
  EXPECT_TRUE(CSSColor::Parse("rgba(256, -1, 123, 0.5)", color));
  EXPECT_EQ(CSSColor(255, 0, 123, 0.5), color);
  EXPECT_TRUE(CSSColor::Parse("rgb(256, -1, 123)", color));
  EXPECT_EQ(CSSColor(255, 0, 123, 1), color);
  EXPECT_TRUE(CSSColor::Parse("#ff00ff", color));
  EXPECT_EQ(CSSColor(255, 0, 255, 1), color);
}
}  // namespace lynx