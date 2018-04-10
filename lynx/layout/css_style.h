// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_LAYOUT_CSS_STYLE_H_
#define LYNX_LAYOUT_CSS_STYLE_H_

#include <errno.h>
#include <stdlib.h>

#include <cmath>
#include <map>
#include <string>

#include "base/compiler_specific.h"
#include "base/string/string_number_convert.h"
#include "base/string/string_utils.h"

#include "layout/css_color.h"
#include "layout/css_layout.h"
#include "layout/css_type.h"


namespace lynx {
const static std::string kPX = "px";
class CSSStyleConfig;
class CSSStyle {
 public:
  CSSStyle();
  CSSStyle(CSSStyleConfig* config, double density, double screen_width, double zoom_ratio);

  ~CSSStyle();

  friend class CSSStaticLayout;

  bool SetValue(const std::string& name, const std::string& value);

  double ClampWidth() const;

  double ClampHeight() const;

  double ClampWidth(double width) const;

  double ClampHeight(double height) const;

  double ClampExactHeight(double height) const;

  double ClampExactWidth(double width) const;

  int width() const { return width_; }

  int height() const { return height_; }

  int margin_left() const { return margin_left_; }

  int margin_top() const { return margin_top_; }

  int margin_right() const { return margin_right_; }

  int margin_bottom() const { return margin_bottom_; }

  CSSStyleType css_position_type() { return css_position_type_; }

  CSSStyleType flex_direction() { return flex_direction_; }

  void set_flex_direction(CSSStyleType flex_direction) {
    flex_direction_ = flex_direction;
  }

  CSSColor& background_color() { return background_color_; }

  int opacity() { return opacity_; }

  int border_radius() { return border_radius_; }

  int border_width() { return border_width_; }

  static void Initialize(CSSStyleConfig* config);

  friend class CSSStaticLayout;

 private:
  double ClampWidthInner(double width) const;

  double ClampHeightInner(double height) const;

  bool ToPx(const std::string& value, double& px);

  void Reset();

  void SetWidth(const std::string& value) {
    if (UNLIKELY(!ToPx(value, width_))) {
      width_ = CSS_UNDEFINED;
    }
  }

  void SetHeight(const std::string& value) {
    if (UNLIKELY(!ToPx(value, height_))) {
      height_ = CSS_UNDEFINED;
    }
  }

  void SetLeft(const std::string& value) {
    if (UNLIKELY(!ToPx(value, left_))) {
      left_ = CSS_UNDEFINED;
    }
  }

  void SetRight(const std::string& value) {
    if (UNLIKELY(!ToPx(value, right_))) {
      right_ = CSS_UNDEFINED;
    }
  }

  void SetTop(const std::string& value) {
    if (UNLIKELY(!ToPx(value, top_))) {
      top_ = CSS_UNDEFINED;
    }
  }

  void SetBottom(const std::string& value) {
    if (UNLIKELY(!ToPx(value, bottom_))) {
      bottom_ = CSS_UNDEFINED;
    }
  }

  void SetMaxWidth(const std::string& value) {
    if (UNLIKELY(!ToPx(value, max_width_))) {
      max_width_ = CSS_UNDEFINED;
    }
  }

  void SetMaxHeight(const std::string& value) {
    if (UNLIKELY(!ToPx(value, max_height_))) {
      max_height_ = CSS_UNDEFINED;
    }
  }

  void SetMarginLeft(const std::string& value) {
    if (UNLIKELY(!ToPx(value, margin_left_))) {
      margin_left_ = 0;
    }
  }

  void SetMarginRight(const std::string& value) {
    if (UNLIKELY(!ToPx(value, margin_right_))) {
      margin_right_ = 0;
    }
  }

  void SetMarginTop(const std::string& value) {
    if (UNLIKELY(!ToPx(value, margin_top_))) {
      margin_top_ = 0;
    }
  }

  void SetMarginBottom(const std::string& value) {
    if (UNLIKELY(!ToPx(value, margin_bottom_))) {
      margin_bottom_ = 0;
    }
  }

  void SetPaddingLeft(const std::string& value) {
    if (UNLIKELY(!ToPx(value, padding_left_))) {
      padding_left_ = 0;
    }
  }

  void SetPaddingRight(const std::string& value) {
    if (UNLIKELY(!ToPx(value, padding_right_))) {
      padding_right_ = 0;
    }
  }

  void SetPaddingTop(const std::string& value) {
    if (UNLIKELY(!ToPx(value, padding_top_))) {
      padding_top_ = 0;
    }
  }

  void SetPaddingBottom(const std::string& value) {
    if (UNLIKELY(!ToPx(value, padding_bottom_))) {
      padding_bottom_ = 0;
    }
  }

  void SetMinWidth(const std::string& value) {
    if (UNLIKELY(!ToPx(value, min_width_))) {
      min_width_ = 0;
    }
  }

  void SetMinHeight(const std::string& value) {
    if (UNLIKELY(!ToPx(value, min_height_))) {
      min_height_ = 0;
    }
  }

  void SetVisible(const std::string& value) {
    if (UNLIKELY(!ToVisibleType(value, visible_))) {
    }
  }

  void SetBackgroundColor(const std::string& value) {
    if (UNLIKELY(!CSSColor::Parse(value, background_color_))) {
      background_color_ = {0, 0, 0, 0};
    }
  }

  void SetFontColor(const std::string& value) {
    if (UNLIKELY(!CSSColor::Parse(value, font_color_))) {
      font_color_ = {0, 0, 0, 1.0f};
    }
  }

  void SetBorderWidth(const std::string& value) {
    if (UNLIKELY(!ToPx(value, border_width_))) {
      border_width_ = 0;
    }
  }

  void SetBorderColor(const std::string& value) {
    if (UNLIKELY(!CSSColor::Parse(value, border_color_))) {
      border_color_ = {0, 0, 0, 1.0f};
    }
  }

  void SetBorderRadius(const std::string& value) {
    if (UNLIKELY(!ToPx(value, border_radius_))) {
      border_radius_ = 0;
    }
  }

  void SetOpacity(const std::string& value) {
    if (UNLIKELY(!base::StringToDouble(value, opacity_))) {
      opacity_ = 1;
    }
    opacity_ *= 255;
  }

  void SetFlex(const std::string& value) {
    if (UNLIKELY(!base::StringToDouble(value, flex_))) {
      flex_ = 1;
    }
  }

  void SetFlexDirection(const std::string& value) {
    if (UNLIKELY(!ToFlexDirectionType(value, flex_direction_))) {
    }
  }

  void SetFlexWrap(const std::string& value) {
    if (UNLIKELY(!ToFlexWrapType(value, flex_wrap_))) {
    }
  }

  void SetFlexJustify(const std::string& value) {
    if (UNLIKELY(!ToFlexJustifyType(value, flex_justify_content_))) {
    }
  }

  void SetFlexAlignItem(const std::string& value) {
    if (UNLIKELY(!ToFlexAlignType(value, flex_align_items_))) {
    }
  }

  void SetFlexAlignSelf(const std::string& value) {
    if (UNLIKELY(!ToFlexAlignType(value, flex_align_self_))) {
    }
  }

  void SetPositionType(const std::string& value) {
    if (UNLIKELY(!ToPositionType(value, css_position_type_))) {
    }
  }

  void SetDisplayType(const std::string& value) {
    if (UNLIKELY(!ToDisplayType(value, css_display_type_))) {
    }
  }

  void SetObjectFit(const std::string& value) {
    if (UNLIKELY(!ToObjectFitType(value, css_object_fit_))) {
    }
  }

  void SetZIndex(const std::string& value) {
    double zindex = 0;
    if (UNLIKELY(!base::StringToDouble(value, zindex))) {
      zindex = 0;
    }
    zindex_ = round(zindex);
  }

  void SetLineHeight(const std::string& value) {
    if (UNLIKELY(!ToPx(value, line_height_))) {
      line_height_ = 0;
    }
  }

  void SetFontSize(const std::string& value) {
    if (UNLIKELY(!ToPx(value, font_size_))) {
      font_size_ = 14 * density_;
    }
  }

  void SetFontWeight(const std::string& value) {
    if (UNLIKELY(!ToTextFontWeightType(value, font_weight_))) {
    }
  }

  void setTextAlign(const std::string& value) {
    if (UNLIKELY(!ToTextAlignType(value, text_align_))) {
    }
  }

  void SetTextWhiteSpace(const std::string& value) {
    if (UNLIKELY(!ToTextWhiteSpaceType(value, text_white_space_))) {
    }
  }

  void SetTextOverflow(const std::string& value) {
    if (UNLIKELY(!ToTextOverflowType(value, text_overflow_))) {
    }
  }

  void SetTextDecoration(const std::string& value) {
    if (UNLIKELY(!ToTextDecorationType(value, text_decoration_))) {
    }
  }

  void SetPointerEvents(const std::string& value) {
    if (UNLIKELY(!ToPointerEventsType(value, pointer_events_))) {
    }
  }

  void SetBackgroundImage(const std::string& value) {
    auto start = value.find('(');
    auto end = value.find(')');
    if (start != std::string::npos && end != std::string::npos) {
      background_image_ = value.substr(start + 1, end - start - 1);
    } else {
      background_image_ = "";
    }
  }

  void SetBackgroundRepeat(const std::string& value) {
    if (UNLIKELY(!ToBackgroundImageRepeatType(value, background_repeat_))) {
    }
  }

  void SetBackgroundSize(const std::string& value) {
    std::vector<std::string> split_result;
    base::SplitString(value, ' ', split_result);
    if (UNLIKELY(split_result.size() > 0 && !ToPx(split_result[0], background_width_))) {
        background_width_ = CSS_UNDEFINED;
    }
    if (split_result.size() > 1 && UNLIKELY(!ToPx(split_result[1], background_height_))) {
        background_height_ = CSS_UNDEFINED;
    }
  }

  void SetBackgroundPosition(const std::string& value) {
    std::vector<std::string> split_result;
    base::SplitString(value, ' ', split_result);
    if (UNLIKELY(split_result.size() > 0 && !ToPx(split_result[0], background_position_x_))) {
        background_position_x_ = 0;
    }
    if (split_result.size() > 1 && UNLIKELY(!ToPx(split_result[1], background_position_y_))) {
        background_position_y_ = 0;
    }
  }

  void SetBackgroundPositionX(const std::string& value) {
      if (UNLIKELY(!ToPx(value, background_position_x_))) {
          background_position_x_ = 0;
      }
  }

  void SetBackgroundPositionY(const std::string& value) {
      if (UNLIKELY(!ToPx(value, background_position_y_))) {
          background_position_y_ = 0;
      }
  }

 public:
  // base css style
  double width_;
  double height_;
  double left_;
  double right_;
  double top_;
  double bottom_;
  double min_width_;
  double max_width_;
  double min_height_;
  double max_height_;
  double margin_left_;
  double margin_right_;
  double margin_top_;
  double margin_bottom_;
  double padding_left_;
  double padding_right_;
  double padding_top_;
  double padding_bottom_;

  CSSStyleType visible_;
  // for view
  CSSColor background_color_;
  double border_width_;
  CSSColor border_color_;
  double border_radius_;
  double opacity_;
  std::string background_image_;
  CSSStyleType background_repeat_;
  double background_width_;
  double background_height_;
  double background_position_x_;
  double background_position_y_;

  // for text
  CSSColor font_color_;
  double font_size_;
  TextStyleType font_weight_;
  TextStyleType text_overflow_;
  TextStyleType text_white_space_;
  TextStyleType text_align_;
  TextStyleType text_decoration_;
  double line_height_;

  // flex css style
  double flex_;
  CSSStyleType flex_direction_;
  CSSStyleType flex_wrap_;
  CSSStyleType flex_justify_content_;
  CSSStyleType flex_align_items_;

  // CSSFlexAlign     mFlexAlignContents;
  CSSStyleType flex_align_self_;

  // css position type
  CSSStyleType css_position_type_;

  // css display type
  CSSStyleType css_display_type_;

  // image object fit
  ImageStyleType css_object_fit_;

  CSSStyleType pointer_events_;

  int zindex_;

  CSSStyleConfig* config_;
  double density_;
  double screen_width_;
  double zoom_ratio_;
};

}  // namespace lynx
#endif  // LYNX_LAYOUT_CSS_STYLE_H_
