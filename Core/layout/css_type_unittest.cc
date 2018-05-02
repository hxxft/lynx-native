#include "layout/css_type.h"

#include "gtest/gtest.h"

namespace lynx {
TEST(CSSTypeTest, ToVisibleTypeTest) {
  CSSStyleType type;
  EXPECT_TRUE(ToVisibleType("visible", type));
  EXPECT_EQ(CSS_VISIBLE, type);
  EXPECT_TRUE(ToVisibleType("hidden", type));
  EXPECT_EQ(CSS_HIDDEN, type);
  EXPECT_FALSE(ToVisibleType("test", type));
  EXPECT_EQ(CSS_VISIBLE, type);
}

TEST(CSSTypeTest, ToDisplayTypeTest) {
  CSSStyleType type;
  EXPECT_TRUE(ToDisplayType("none", type));
  EXPECT_EQ(CSS_DISPLAY_NONE, type);
  EXPECT_TRUE(ToDisplayType("flex", type));
  EXPECT_EQ(CSS_DISPLAY_FLEX, type);
  EXPECT_FALSE(ToDisplayType("test", type));
  EXPECT_EQ(CSS_DISPLAY_FLEX, type);
}

TEST(CSSTypeTest, ToFlexAlignTypeTest) {
  CSSStyleType type;
  EXPECT_TRUE(ToFlexAlignType("flex-start", type));
  EXPECT_EQ(CSSFLEX_ALIGN_FLEX_START, type);
  EXPECT_TRUE(ToFlexAlignType("flex-end", type));
  EXPECT_EQ(CSSFLEX_ALIGN_FLEX_END, type);
  EXPECT_TRUE(ToFlexAlignType("center", type));
  EXPECT_EQ(CSSFLEX_ALIGN_CENTER, type);
  EXPECT_TRUE(ToFlexAlignType("stretch", type));
  EXPECT_EQ(CSSFLEX_ALIGN_STRETCH, type);
  EXPECT_FALSE(ToFlexAlignType("test", type));
  EXPECT_EQ(CSSFLEX_ALIGN_AUTO, type);
}

TEST(CSSTypeTest, ToFlexDirectionTypeTest) {
  CSSStyleType type;
  EXPECT_TRUE(ToFlexDirectionType("column", type));
  EXPECT_EQ(CSSFLEX_DIRECTION_COLUMN, type);
  EXPECT_TRUE(ToFlexDirectionType("row", type));
  EXPECT_EQ(CSSFLEX_DIRECTION_ROW, type);
  EXPECT_FALSE(ToFlexDirectionType("test", type));
  EXPECT_EQ(CSSFLEX_DIRECTION_COLUMN, type);
}

TEST(CSSTypeTest, ToFlexJustifyTypeTest) {
  CSSStyleType type;
  EXPECT_TRUE(ToFlexJustifyType("flex-start", type));
  EXPECT_EQ(CSSFLEX_JUSTIFY_FLEX_START, type);
  EXPECT_TRUE(ToFlexJustifyType("center", type));
  EXPECT_EQ(CSSFLEX_JUSTIFY_FLEX_CENTER, type);
  EXPECT_TRUE(ToFlexJustifyType("flex-end", type));
  EXPECT_EQ(CSSFLEX_JUSTIFY_FLEX_END, type);
  EXPECT_TRUE(ToFlexJustifyType("space-between", type));
  EXPECT_EQ(CSSFLEX_JUSTIFY_SPACE_BETWEEN, type);
  EXPECT_TRUE(ToFlexJustifyType("space-around", type));
  EXPECT_EQ(CSSFLEX_JUSTIFY_SPACE_AROUND, type);

  EXPECT_FALSE(ToFlexJustifyType("test", type));
  EXPECT_EQ(CSSFLEX_JUSTIFY_FLEX_START, type);
}

TEST(CSSTypeTest, ToFlexWrapTypeTest) {
  CSSStyleType type;
  EXPECT_TRUE(ToFlexWrapType("wrap", type));
  EXPECT_EQ(CSSFLEX_WRAP, type);
  EXPECT_TRUE(ToFlexWrapType("nowrap", type));
  EXPECT_EQ(CSSFLEX_NOWRAP, type);
  EXPECT_FALSE(ToFlexWrapType("test", type));
  EXPECT_EQ(CSSFLEX_NOWRAP, type);
}

TEST(CSSTypeTest, ToPositionTypeTest) {
  CSSStyleType type;
  EXPECT_TRUE(ToPositionType("absolute", type));
  EXPECT_EQ(CSS_POSITION_ABSOLUTE, type);
  EXPECT_TRUE(ToPositionType("relative", type));
  EXPECT_EQ(CSS_POSITION_RELATIVE, type);
  EXPECT_TRUE(ToPositionType("fixed", type));
  EXPECT_EQ(CSS_POSITION_FIXED, type);
  EXPECT_FALSE(ToPositionType("test", type));
  EXPECT_EQ(CSS_POSITION_RELATIVE, type);
}

TEST(CSSTypeTest, ToTextAlignTypeTest) {
  TextStyleType type;
  EXPECT_TRUE(ToTextAlignType("left", type));
  EXPECT_EQ(CSSTEXT_ALIGN_LEFT, type);
  EXPECT_TRUE(ToTextAlignType("right", type));
  EXPECT_EQ(CSSTEXT_ALIGN_RIGHT, type);
  EXPECT_TRUE(ToTextAlignType("center", type));
  EXPECT_EQ(CSSTEXT_ALIGN_CENTER, type);
  EXPECT_FALSE(ToTextAlignType("test", type));
  EXPECT_EQ(CSSTEXT_ALIGN_LEFT, type);
}

TEST(CSSTypeTest, ToTextDecorationTypeTest) {
  TextStyleType type;
  EXPECT_TRUE(ToTextDecorationType("line-through", type));
  EXPECT_EQ(CSSTEXT_TEXTDECORATION_LINETHROUGH, type);
  EXPECT_FALSE(ToTextDecorationType("test", type));
  EXPECT_EQ(CSSTEXT_TEXTDECORATION_NONE, type);
}

TEST(CSSTypeTest, ToTextFontWeightTypeTest) {
  TextStyleType type;
  EXPECT_TRUE(ToTextFontWeightType("normal", type));
  EXPECT_EQ(CSSTEXT_FONT_WEIGHT_NORMAL, type);
  EXPECT_TRUE(ToTextFontWeightType("bold", type));
  EXPECT_EQ(CSSTEXT_FONT_WEIGHT_BOLD, type);
  EXPECT_FALSE(ToTextFontWeightType("test", type));
  EXPECT_EQ(CSSTEXT_FONT_WEIGHT_NORMAL, type);
}
TEST(CSSTypeTest, ToTextOverflowTypeTest) {
  TextStyleType type;
  EXPECT_TRUE(ToTextOverflowType("ellipsis", type));
  EXPECT_EQ(CSSTEXT_OVERFLOW_ELLIPSIS, type);
  EXPECT_FALSE(ToTextOverflowType("test", type));
  EXPECT_EQ(CSSTEXT_OVERFLOW_NONE, type);
}
TEST(CSSTypeTest, ToTextWhiteSpaceTypeTest) {
  TextStyleType type;
  EXPECT_TRUE(ToTextWhiteSpaceType("nowrap", type));
  EXPECT_EQ(CSSTEXT_WHITESPACE_NOWRAP, type);
  EXPECT_FALSE(ToTextWhiteSpaceType("test", type));
  EXPECT_EQ(CSSTEXT_WHITESPACE_NORMAL, type);
}

TEST(CSSTypeTest, ToObjectFitTypeTest) {
  ImageStyleType type;
  EXPECT_TRUE(ToObjectFitType("fill", type));
  EXPECT_EQ(CSSIMAGE_OBJECT_FIT_FILL, type);
  EXPECT_TRUE(ToObjectFitType("contain", type));
  EXPECT_EQ(CSSIMAGE_OBJECT_FIT_CONTAIN, type);
  EXPECT_TRUE(ToObjectFitType("cover", type));
  EXPECT_EQ(CSSIMAGE_OBJECT_FIT_COVER, type);
  EXPECT_FALSE(ToObjectFitType("test", type));
  EXPECT_EQ(CSSIMAGE_OBJECT_FIT_COVER, type);
}

}  // namespace lynx