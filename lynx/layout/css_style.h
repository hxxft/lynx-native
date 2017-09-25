// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_LAYOUT_CSS_STYLE_H_
#define LYNX_LAYOUT_CSS_STYLE_H_

#include <cstdlib>
#include <cmath>
#include <map>
#include <string>

#include "layout/css_type.h"
#include "layout/css_layout.h"
#include "layout/css_color.h"

namespace base {
class Convert;
}

namespace lynx {
    const static std::string kPX = "px";
class CSSStyleConfig;
class CSSStyle {
 public:
    CSSStyle();

    ~CSSStyle();

    friend class CSSStaticLayout;

    void SetValue(const std::string& name, const std::string& value);

    double ClampWidth() const;

    double ClampHeight() const;

    double ClampWidth(double width) const;

    double ClampHeight(double height) const;

    double ClampExactHeight(double height) const;

    double ClampExactWidth(double width) const;

    int width() const {
        return width_;
    }

    int height() const {
        return height_;
    }

    int margin_left() const {
        return margin_left_;
    }

    int margin_top() const {
        return margin_top_;
    }

    int margin_right() const {
        return margin_right_;
    }

    int margin_bottom() const {
        return margin_bottom_;
    }

    CSSStyleType css_position_type() {
        return css_position_type_;
    }

    CSSStyleType flex_direction() {
        return flex_direction_;
    }

    void set_flex_direction(CSSStyleType flex_direction) {
        flex_direction_ = flex_direction;
    }

    CSSColor& background_color() {
        return background_color_;
    }
    
    int opacity() {
        return opacity_;
    }
    
    int border_radius() {
        return border_radius_;
    }
    
    int border_width() {
        return border_width_;
    }
    
    static void Initialize(CSSStyleConfig* config);

    friend class CSSStaticLayout;
    friend class base::Convert;

 private:

    double ClampWidthInner(double width) const;

    double ClampHeightInner(double height) const;

    int ToPx(const std::string& value) {
        if(value.find(kPX) == std::string::npos) {
            return round(atof(value.c_str()) * screen_width_ * density_ / 750 );
        }else {
            return round(atof(value.c_str()) * density_);
        }
    }

    void SetWidth(const std::string& value) {
        if (value.empty()) {
            width_ = CSS_UNDEFINED;
        } else {
            width_ = ToPx(value);
        }
    }

    void SetHeight(const std::string& value) {
        if (value.empty()) {
            height_ = CSS_UNDEFINED;
        } else {
            height_ = ToPx(value);
        }
    }

    void SetLeft(const std::string& value) {
        if (value.empty()) {
            left_ = CSS_UNDEFINED;
        } else {
            left_ = ToPx(value);
        }
    }

    void SetRight(const std::string& value) {
        if (value.empty()) {
            right_ = CSS_UNDEFINED;
        } else {
            right_ = ToPx(value);
        }
    }

    void SetTop(const std::string& value) {
        if (value.empty()) {
            top_ = CSS_UNDEFINED;
        } else {
            top_ = ToPx(value);
        }
    }

    void SetBottom(const std::string& value) {
        if (value.empty()) {
            bottom_ = CSS_UNDEFINED;
        } else {
            bottom_ = ToPx(value);
        }
    }

    void SetMaxWidth(const std::string& value) {
        if (value.empty()) {
            max_width_ = CSS_UNDEFINED;
        } else {
            max_width_ = ToPx(value);
        }
    }

    void SetMaxHeight(const std::string& value) {
        if (value.empty()) {
            max_height_ = CSS_UNDEFINED;
        } else {
            max_height_ = ToPx(value);
        }
    }

    void SetMarginLeft(const std::string& value) {
        if (value.empty()) {
            margin_left_ = 0;
        } else {
            margin_left_ = ToPx(value);
        }
    }

    void SetMarginRight(const std::string& value) {
        if (value.empty()) {
            margin_right_ = 0;
        } else {
            margin_right_ = ToPx(value);
        }
    }

    void SetMarginTop(const std::string& value) {
        if (value.empty()) {
            margin_top_ = 0;
        } else {
            margin_top_ = ToPx(value);
        }
    }

    void SetMarginBottom(const std::string& value) {
        if (value.empty()) {
            margin_bottom_ = 0;
        } else {
            margin_bottom_ = ToPx(value);
        }
    }

    void SetPaddingLeft(const std::string& value) {
        if (value.empty()) {
            padding_left_ = 0;
        } else {
            padding_left_ = ToPx(value);
        }
    }

    void SetPaddingRight(const std::string& value) {
        if (value.empty()) {
            padding_right_ = 0;
        } else {
            padding_right_ = ToPx(value);
        }
    }

    void SetPaddingTop(const std::string& value) {
        if (value.empty()) {
            padding_top_ = 0;
        } else {
            padding_top_ = ToPx(value);
        }
    }

    void SetPaddingBottom(const std::string& value) {
        if (value.empty()) {
            padding_bottom_ = 0;
        } else {
            padding_bottom_ = ToPx(value);
        }
    }

    void SetMinWidth(const std::string& value) {
        if (value.empty()) {
            min_width_ = 0;
        } else {
            min_width_ = ToPx(value);
        }
    }

    void SetMinHeight(const std::string& value) {
        if (value.empty()) {
            min_height_ = 0;
        } else {
            min_height_ = ToPx(value);
        }
    }

    void SetVisible(const std::string& value) {
        visible_ = ToVisibleType(value);
    }

    void SetBackgroundColor(const std::string& value) {
        if (!value.empty()) {
            background_color_ = CSSColor::Parse(value);
        }
    }

    void setFontColor(const std::string& value) {
        if (!value.empty()) {
            font_color_ = CSSColor::Parse(value);
        }
    }

    void SetBorderWidth(const std::string& value) {
        if (value.empty()) {
            border_width_ = 0;
        } else {
            border_width_ = ToPx(value);
        }
    }

    void SetBorderColor(const std::string& value) {
        if (!value.empty()) {
            border_color_ = CSSColor::Parse(value);
        }
    }

    void SetBorderRadius(const std::string& value) {
        if (value.empty()) {
            border_radius_ = 0;
        } else {
            border_radius_ = ToPx(value);
        }
    }

    void SetOpacity(const std::string& value) {
        if (value.empty()) {
            opacity_ = 255;
        } else {
            opacity_ = atof(value.c_str()) * 255;
        }
    }

    void SetFlex(const std::string& value) {
        if (value.empty()) {
            flex_ = 0;
        } else {
            flex_ = (int) atof(value.c_str());
        }
    }

    void SetFlexDirection(const std::string& value) {
        flex_direction_ = ToFlexDirectionType(value);
    }

    void SetFlexWrap(const std::string& value) {
        flex_wrap_ = ToFlexWrapType(value);
    }

    void SetFlexJustify(const std::string& value) {
        flex_justify_content_ = ToFlexJustifyType(value);
    }

    void SetFlexAlignItem(const std::string& value) {
        flex_align_items_ = ToFlexAlignType(value);
    }

    void SetFlexAlignSelf(const std::string& value) {
        flex_align_self_ = ToFlexAlignType(value);
    }

    void SetPositionType(const std::string& value) {
        css_position_type_ = ToPositionType(value);
    }

    void SetDisplayType(const std::string& value) {
        css_display_type_ = ToDisplayType(value);
    }

    void SetObjectFit(const std::string& value) {
        css_object_fit_ = ToObjectFitType(value);
    }

    void SetZIndex(const std::string& value) {
        if (value.empty()) {
            zindex_ = 0;
        } else {
            zindex_ = (int) atof(value.c_str());
        }
    }

    void SetLineHeight(const std::string& value) {
        if (value.empty()) {
            line_height_ = 0;
        } else {
            line_height_ = ToPx(value);
        }
    }

    void SetFontSize(const std::string& value) {
        if (value.empty()) {
            font_size_ = 14 * density_;
        } else {
            font_size_ = ToPx(value);
        }
    }

    void SetFontWeight(const std::string& value) {
        font_weight_ = ToTextFontWeightType(value);
    }

    void setTextAlign(const std::string& value) {
        text_align_ = ToTextAlignType(value);
    }

    void SetTextWhiteSpace(const std::string& value) {
        text_white_space_ = ToTextWhiteSpaceType(value);
    }

    void SetTextOverflow(const std::string& value) {
        text_overflow_ = ToTextOverflowType(value);
    }

    void SetTextDecoration(const std::string& value) {
        text_decoration_ = ToTextDecorationType(value);
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
    int  flex_;
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

    int zindex_;

    CSSStyleConfig* config_;
    double density_;
    double screen_width_;
};

}  // namespace lynx
#endif  // LYNX_LAYOUT_CSS_STYLE_H_
