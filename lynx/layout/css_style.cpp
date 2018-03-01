// Copyright 2017 The Lynx Authors. All rights reserved.

#include "layout/css_style.h"

#include "config/global_config_data.h"
#include "layout/css_style_config.h"

namespace lynx {

void CSSStyle::Initialize(CSSStyleConfig* config) {
  CSSStyleConfig::StyleFuncMap* func_map = config->func_map();
  (*func_map)["width"] = &CSSStyle::SetWidth;
  (*func_map)["height"] = &CSSStyle::SetHeight;
  (*func_map)["left"] = &CSSStyle::SetLeft;
  (*func_map)["right"] = &CSSStyle::SetRight;
  (*func_map)["top"] = &CSSStyle::SetTop;
  (*func_map)["bottom"] = &CSSStyle::SetBottom;
  (*func_map)["minWidth"] = &CSSStyle::SetMinWidth;
  (*func_map)["min-width"] = &CSSStyle::SetMinWidth;
  (*func_map)["maxWidth"] = &CSSStyle::SetMaxWidth;
  (*func_map)["max-width"] = &CSSStyle::SetMaxWidth;
  (*func_map)["minHeight"] = &CSSStyle::SetMinHeight;
  (*func_map)["min-height"] = &CSSStyle::SetMinHeight;
  (*func_map)["maxHeight"] = &CSSStyle::SetMaxHeight;
  (*func_map)["max-height"] = &CSSStyle::SetMaxHeight;
  (*func_map)["marginLeft"] = &CSSStyle::SetMarginLeft;
  (*func_map)["margin-left"] = &CSSStyle::SetMarginLeft;
  (*func_map)["marginRight"] = &CSSStyle::SetMarginRight;
  (*func_map)["margin-right"] = &CSSStyle::SetMarginRight;
  (*func_map)["marginTop"] = &CSSStyle::SetMarginTop;
  (*func_map)["margin-top"] = &CSSStyle::SetMarginTop;
  (*func_map)["marginBottom"] = &CSSStyle::SetMarginBottom;
  (*func_map)["margin-bottom"] = &CSSStyle::SetMarginBottom;
  (*func_map)["paddingLeft"] = &CSSStyle::SetPaddingLeft;
  (*func_map)["padding-left"] = &CSSStyle::SetPaddingLeft;
  (*func_map)["paddingRight"] = &CSSStyle::SetPaddingRight;
  (*func_map)["padding-right"] = &CSSStyle::SetPaddingRight;
  (*func_map)["paddingTop"] = &CSSStyle::SetPaddingTop;
  (*func_map)["padding-top"] = &CSSStyle::SetPaddingTop;
  (*func_map)["paddingBottom"] = &CSSStyle::SetPaddingBottom;
  (*func_map)["padding-bottom"] = &CSSStyle::SetPaddingBottom;
  (*func_map)["visible"] = &CSSStyle::SetVisible;
  (*func_map)["backgroundColor"] = &CSSStyle::SetBackgroundColor;
  (*func_map)["background-color"] = &CSSStyle::SetBackgroundColor;
  (*func_map)["color"] = &CSSStyle::SetFontColor;
  (*func_map)["borderWidth"] = &CSSStyle::SetBorderWidth;
  (*func_map)["border-width"] = &CSSStyle::SetBorderWidth;
  (*func_map)["borderColor"] = &CSSStyle::SetBorderColor;
  (*func_map)["border-color"] = &CSSStyle::SetBorderColor;
  (*func_map)["borderRadius"] = &CSSStyle::SetBorderRadius;
  (*func_map)["border-radius"] = &CSSStyle::SetBorderRadius;
  (*func_map)["opacity"] = &CSSStyle::SetOpacity;
  (*func_map)["flex"] = &CSSStyle::SetFlex;
  (*func_map)["flexDirection"] = &CSSStyle::SetFlexDirection;
  (*func_map)["flex-direction"] = &CSSStyle::SetFlexDirection;
  (*func_map)["flexWrap"] = &CSSStyle::SetFlexWrap;
  (*func_map)["flex-wrap"] = &CSSStyle::SetFlexWrap;
  (*func_map)["justifyContent"] = &CSSStyle::SetFlexJustify;
  (*func_map)["justify-content"] = &CSSStyle::SetFlexJustify;
  (*func_map)["alignItems"] = &CSSStyle::SetFlexAlignItem;
  (*func_map)["align-items"] = &CSSStyle::SetFlexAlignItem;
  (*func_map)["alignSelf"] = &CSSStyle::SetFlexAlignSelf;
  (*func_map)["align-self"] = &CSSStyle::SetFlexAlignSelf;
  (*func_map)["position"] = &CSSStyle::SetPositionType;
  (*func_map)["fontWeight"] = &CSSStyle::SetFontWeight;
  (*func_map)["font-weight"] = &CSSStyle::SetFontWeight;
  (*func_map)["fontSize"] = &CSSStyle::SetFontSize;
  (*func_map)["font-size"] = &CSSStyle::SetFontSize;
  (*func_map)["textOverflow"] = &CSSStyle::SetTextOverflow;
  (*func_map)["text-overflow"] = &CSSStyle::SetTextOverflow;
  (*func_map)["whiteSpace"] = &CSSStyle::SetTextWhiteSpace;
  (*func_map)["white-space"] = &CSSStyle::SetTextWhiteSpace;
  (*func_map)["textAlign"] = &CSSStyle::setTextAlign;
  (*func_map)["text-align"] = &CSSStyle::setTextAlign;
  (*func_map)["textDecoration"] = &CSSStyle::SetTextDecoration;
  (*func_map)["text-decoration"] = &CSSStyle::SetTextDecoration;
  (*func_map)["display"] = &CSSStyle::SetDisplayType;
  (*func_map)["objectFit"] = &CSSStyle::SetObjectFit;
  (*func_map)["object-fit"] = &CSSStyle::SetObjectFit;
  (*func_map)["zIndex"] = &CSSStyle::SetZIndex;
  (*func_map)["z-index"] = &CSSStyle::SetZIndex;
  (*func_map)["line-height"] = &CSSStyle::SetLineHeight;
  (*func_map)["lineHeight"] = &CSSStyle::SetLineHeight;
}

CSSStyle::CSSStyle() {
  config_ = config::GlobalConfigData::GetInstance()->style_config();
  density_ = config::GlobalConfigData::GetInstance()->screen_density();
  screen_width_ = config::GlobalConfigData::GetInstance()->screen_width();
  zoom_reference_ = config::GlobalConfigData::GetInstance()->zoom_reference();

  Reset();
}

CSSStyle::CSSStyle(CSSStyleConfig* config,
                   double density,
                   double screen_width) {
  config_ = config;
  density_ = density;
  screen_width_ = screen_width;

  Reset();
}

CSSStyle::~CSSStyle() {}

void CSSStyle::Reset() {
  width_ = CSS_UNDEFINED;
  height_ = CSS_UNDEFINED;
  left_ = CSS_UNDEFINED;
  right_ = CSS_UNDEFINED;
  top_ = CSS_UNDEFINED;
  bottom_ = CSS_UNDEFINED;
  max_width_ = CSS_UNDEFINED;
  max_height_ = CSS_UNDEFINED;

  margin_left_ = 0;
  margin_right_ = 0;
  margin_top_ = 0;
  margin_bottom_ = 0;
  padding_left_ = 0;
  padding_right_ = 0;
  padding_top_ = 0;
  padding_bottom_ = 0;
  min_width_ = 0;
  min_height_ = 0;

  visible_ = CSS_VISIBLE;
  background_color_ = {0, 0, 0, 0};  // Color.parse("#00000000");
  font_color_ = {0, 0, 0, 1.0f};     // Color.parse("#FF000000");
  border_width_ = 0;
  border_color_ = {0, 0, 0, 1.0f};  // Color.parse("#FF000000");
  border_radius_ = 0;               // CSS_UNDEFINED;
  opacity_ = 255;

  flex_ = 0;
  flex_direction_ = CSSFLEX_DIRECTION_ROW;
  flex_wrap_ = CSSFLEX_NOWRAP;
  flex_justify_content_ = CSSFLEX_JUSTIFY_FLEX_START;
  flex_align_items_ = CSSFLEX_ALIGN_STRETCH;
  flex_align_self_ = CSSFLEX_ALIGN_AUTO;

  css_position_type_ = CSS_POSITION_RELATIVE;
  css_display_type_ = CSS_DISPLAY_FLEX;

  font_size_ = 14 * density_;
  font_weight_ = CSSTEXT_FONT_WEIGHT_NORMAL;

  text_align_ = CSSTEXT_ALIGN_LEFT;
  text_white_space_ = CSSTEXT_WHITESPACE_NORMAL;
  text_overflow_ = CSSTEXT_OVERFLOW_NONE;
  text_decoration_ = CSSTEXT_TEXTDECORATION_NONE;
  line_height_ = CSS_UNDEFINED;

  css_object_fit_ = CSSIMAGE_OBJECT_FIT_COVER;
  zindex_ = 0;
}

bool CSSStyle::ToPx(const std::string& value, double& px) {
  bool is_px = false;
  std::string px_str = value;
  int start = value.length() - kPX.length();
  if (start > 0 && value.find(kPX, start - 1) != std::string::npos) {
    is_px = true;
    px_str = value.substr(0, start);
  }

  double d;
  if (UNLIKELY(!base::StringToDouble(px_str, d))) {
    return false;
  }
  if (is_px) {
    px = round(d * density_);
  } else {
    px = round(d * density_ * screen_width_ / zoom_reference_);
  }
  return true;
}

bool CSSStyle::SetValue(const std::string& name, const std::string& value) {
  CSSStyleConfig::StyleFuncMap* func_map = config_->func_map();
  CSSStyleConfig::StyleFuncMap::iterator iter = func_map->find(name);

  if (iter == func_map->end())
    return false;
  CSSStyleConfig::StyleFunc func = iter->second;
  (this->*func)(value);
  return true;
}

double CSSStyle::ClampWidthInner(double width) const {
  if (width < min_width_)
    width = min_width_;
  if (width > max_width_)
    width = max_width_;

  double extra = padding_left_ + padding_right_ + border_width_ * 2;
  width = extra > width ? extra : width;

  return width;
}

double CSSStyle::ClampHeightInner(double height) const {
  if (height < min_height_)
    height = min_height_;
  if (height > max_height_)
    height = max_height_;

  double extra = padding_top_ + padding_bottom_ + border_width_ * 2;
  height = extra > height ? extra : height;

  return height;
}

double CSSStyle::ClampWidth() const {
  double width = width_;
  return ClampWidthInner(width);
}

double CSSStyle::ClampHeight() const {
  double height = height_;
  return ClampHeightInner(height);
}

double CSSStyle::ClampWidth(double width) const {
  if (!CSS_IS_UNDEFINED(width_)) {
    width = width_;
  }

  if (CSS_IS_UNDEFINED(width)) {
    width = 0;
  }

  return ClampWidthInner(width);
}

double CSSStyle::ClampHeight(double height) const {
  if (!CSS_IS_UNDEFINED(height_)) {
    height = height_;
  }

  if (CSS_IS_UNDEFINED(height)) {
    height = 0;
  }

  return ClampHeightInner(height);
}

double CSSStyle::ClampExactHeight(double height) const {
  if (CSS_IS_UNDEFINED(height)) {
    height = 0;
  }
  return ClampHeightInner(height);
}

double CSSStyle::ClampExactWidth(double width) const {
  if (CSS_IS_UNDEFINED(width)) {
    width = 0;
  }
  return ClampWidthInner(width);
}

}  // namespace lynx
