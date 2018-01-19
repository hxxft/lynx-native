
#include "gtest/gtest.h"

#define private public
#define protected public

#include "css_style.h"
#include "css_style_config.h"

namespace lynx {

class CSSStyleTest : public testing::Test {
 public:
  CSSStyleTest() {
    CSSStyle::Initialize(&config_);
    css_style_ = new CSSStyle(&config_, 1.5, 350.0);
  }

  ~CSSStyleTest() { delete css_style_; }

  CSSStyle* style() { return css_style_; }

 private:
  CSSStyleConfig config_;
  CSSStyle* css_style_;
};

TEST_F(CSSStyleTest, ToPXTest) {
  double px0 = 0;
  EXPECT_TRUE(style()->ToPx("750", px0));
  EXPECT_EQ(525, px0);

  EXPECT_TRUE(style()->ToPx("750px", px0));
  EXPECT_EQ(1125, px0);

  EXPECT_FALSE(style()->ToPx("750rpx", px0));
  EXPECT_EQ(1125, px0);
}

#define PixelSetterTest(Setter, Getter, ErrorExpect) \
  TEST_F(CSSStyleTest, Setter##Test) {               \
    style()->Setter("750");                          \
    EXPECT_EQ(525, style()->Getter##_);              \
    style()->Setter("750px");                        \
    EXPECT_EQ(1125, style()->Getter##_);             \
    style()->Setter("750rpx");                       \
    EXPECT_EQ(ErrorExpect, style()->Getter##_);      \
  }

PixelSetterTest(SetWidth, width, CSS_UNDEFINED);
PixelSetterTest(SetHeight, height, CSS_UNDEFINED);
PixelSetterTest(SetLeft, left, CSS_UNDEFINED);
PixelSetterTest(SetRight, right, CSS_UNDEFINED);
PixelSetterTest(SetTop, top, CSS_UNDEFINED);
PixelSetterTest(SetBottom, bottom, CSS_UNDEFINED);
PixelSetterTest(SetMaxWidth, max_width, CSS_UNDEFINED);
PixelSetterTest(SetMaxHeight, max_height, CSS_UNDEFINED);
PixelSetterTest(SetMarginLeft, margin_left, 0);
PixelSetterTest(SetMarginRight, margin_right, 0);
PixelSetterTest(SetMarginTop, margin_top, 0);
PixelSetterTest(SetMarginBottom, margin_bottom, 0);
PixelSetterTest(SetPaddingLeft, padding_left, 0);
PixelSetterTest(SetPaddingRight, padding_right, 0);
PixelSetterTest(SetPaddingTop, padding_top, 0);
PixelSetterTest(SetPaddingBottom, padding_bottom, 0);
PixelSetterTest(SetMinWidth, min_width, 0);
PixelSetterTest(SetMinHeight, min_height, 0);
PixelSetterTest(SetBorderWidth, border_width, 0);
PixelSetterTest(SetBorderRadius, border_radius, 0);
PixelSetterTest(SetLineHeight, line_height, 0);
PixelSetterTest(SetFontSize, font_size, 21);

TEST_F(CSSStyleTest, SetOpacityTest) {
  style()->SetOpacity("0.5");
  EXPECT_EQ(127.5, style()->opacity_);
  style()->SetOpacity("0.5px");
  EXPECT_EQ(255, style()->opacity_);
}

TEST_F(CSSStyleTest, SetFlexTest) {
  style()->SetFlex("1.5");
  EXPECT_EQ(1.5, style()->flex_);
  style()->SetFlex("2.5px");
  EXPECT_EQ(1, style()->flex_);
}

TEST_F(CSSStyleTest, SetZIndexTest) {
  style()->SetZIndex("2");
  EXPECT_EQ(2, style()->zindex_);
  style()->SetZIndex("3px");
  EXPECT_EQ(0, style()->zindex_);
}

TEST_F(CSSStyleTest, SetBorderColorTest) {
  style()->SetBorderColor("red");
  EXPECT_EQ(CSSColor(255, 0, 0, 1), style()->border_color_);
  style()->SetBorderColor("rgba(255, 123, 0, 1)");
  EXPECT_EQ(CSSColor(255, 123, 0, 1), style()->border_color_);
  style()->SetBorderColor("rgb(0, 255, 123)");
  EXPECT_EQ(CSSColor(0, 255, 123, 1), style()->border_color_);
  style()->SetBorderColor("#00ff00");
  EXPECT_EQ(CSSColor(0, 255, 0, 1), style()->border_color_);
  style()->SetBorderColor("test");
  EXPECT_EQ(CSSColor(0, 0, 0, 1), style()->border_color_);
}

}  // namespace lynx

#undef private
#undef protected
