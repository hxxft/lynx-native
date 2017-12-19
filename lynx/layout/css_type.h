// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_LAYOUT_CSS_TYPE_H_
#define LYNX_LAYOUT_CSS_TYPE_H_

#include <limits.h>
#include <string>

namespace lynx {

#define CSS_UNDEFINED 0x7FFFFFFF
#define CSS_MAX (0x7FFFFFFF - 1)
#define CSS_IS_UNDEFINED(VAL) (VAL == INT_MAX)

enum CSSStyleType {
    CSS_DISPLAY_NONE = 0,
    CSS_DISPLAY_FLEX = 1,

    CSSFLEX_ALIGN_AUTO = 2,
    CSSFLEX_ALIGN_FLEX_START = 3,
    CSSFLEX_ALIGN_CENTER = 4,
    CSSFLEX_ALIGN_FLEX_END = 5,
    CSSFLEX_ALIGN_STRETCH = 6,

    CSSFLEX_DIRECTION_COLUMN = 7,
    CSSFLEX_DIRECTION_COLUMN_REVERS = 8,
    CSSFLEX_DIRECTION_ROW = 9,
    CSSFLEX_DIRECTION_ROW_REVERSE = 10,

    CSSFLEX_JUSTIFY_FLEX_START = 11,
    CSSFLEX_JUSTIFY_FLEX_CENTER = 12,
    CSSFLEX_JUSTIFY_FLEX_END = 13,
    CSSFLEX_JUSTIFY_SPACE_BETWEEN = 14,
    CSSFLEX_JUSTIFY_SPACE_AROUND = 15,

    CSSFLEX_WRAP = 16,
    CSSFLEX_NOWRAP = 17,

    CSS_POSITION_RELATIVE =18,
    CSS_POSITION_ABSOLUTE = 19,
    CSS_POSITION_FIXED = 20,

    CSS_VISIBLE = 21,
    CSS_HIDDEN = 22
};

enum TextStyleType {
    CSSTEXT_ALIGN_LEFT = 0,
    CSSTEXT_ALIGN_RIGHT,
    CSSTEXT_ALIGN_CENTER,

    CSSTEXT_TEXTDECORATION_LINETHROUGH,
    CSSTEXT_TEXTDECORATION_NONE,

    CSSTEXT_FONT_WEIGHT_NORMAL,
    CSSTEXT_FONT_WEIGHT_BOLD,

    CSSTEXT_OVERFLOW_ELLIPSIS,
    CSSTEXT_OVERFLOW_NONE,

    CSSTEXT_WHITESPACE_NOWRAP,
    CSSTEXT_WHITESPACE_NORMAL
};

enum ImageStyleType {
    CSSIMAGE_OBJECT_FIT_FILL,
    CSSIMAGE_OBJECT_FIT_CONTAIN,
    CSSIMAGE_OBJECT_FIT_COVER
};

CSSStyleType ToVisibleType(const std::string& value);

CSSStyleType ToDisplayType(const std::string& value);

CSSStyleType ToFlexAlignType(const std::string& value);

CSSStyleType ToFlexDirectionType(const std::string& value);

CSSStyleType ToFlexJustifyType(const std::string& value);

CSSStyleType ToFlexWrapType(const std::string& value);

CSSStyleType ToPositionType(const std::string& value);

TextStyleType ToTextAlignType(const std::string& value);

TextStyleType ToTextDecorationType(const std::string& value);

TextStyleType ToTextFontWeightType(const std::string& value);

TextStyleType ToTextOverflowType(const std::string& value);

TextStyleType ToTextWhiteSpaceType(const std::string& value);

ImageStyleType ToObjectFitType(const std::string& value);
    
    std::string MapCSSType(CSSStyleType type);
    std::string MapTextStyle(TextStyleType type);
    std::string MapImageStyle(ImageStyleType type);

}  // namespace lynx

#endif  // LYNX_LAYOUT_CSS_TYPE_H_
