// Copyright 2017 The Lynx Authors. All rights reserved.

#include "layout/css_type.h"

namespace lynx {
CSSStyleType ToVisibleType(const std::string& value) {
    if (value.compare("visible") == 0) {
        return CSS_VISIBLE;
    } else if (value.compare("hidden") == 0) {
        return CSS_HIDDEN;
    }
    return CSS_VISIBLE;
}

CSSStyleType ToDisplayType(const std::string& value) {
    if (value.compare("none") == 0) {
        return CSS_DISPLAY_NONE;
    } else if (value.compare("flex") == 0) {
        return CSS_DISPLAY_FLEX;
    }
    return CSS_DISPLAY_FLEX;
}

CSSStyleType ToFlexAlignType(const std::string& value) {
    if (value.compare("flex-start") == 0) {
        return CSSFLEX_ALIGN_FLEX_START;
    } else if (value.compare("flex-end") == 0) {
        return CSSFLEX_ALIGN_FLEX_END;
    } else if (value.compare("center") == 0) {
        return CSSFLEX_ALIGN_CENTER;
    } else if (value.compare("stretch") == 0) {
        return CSSFLEX_ALIGN_STRETCH;
    }
    return CSSFLEX_ALIGN_AUTO;
}

CSSStyleType ToFlexDirectionType(const std::string& value) {
    if (value.compare("column") == 0) {
        return CSSFLEX_DIRECTION_COLUMN;
    } else if (value.compare("row") == 0) {
        return CSSFLEX_DIRECTION_ROW;
    }
    return CSSFLEX_DIRECTION_COLUMN;
}

CSSStyleType ToFlexJustifyType(const std::string& value) {
    if (value.compare("flex-start") == 0) {
        return CSSFLEX_JUSTIFY_FLEX_START;
    } else if (value.compare("center") == 0) {
        return CSSFLEX_JUSTIFY_FLEX_CENTER;
    } else if (value.compare("flex-end") == 0) {
        return CSSFLEX_JUSTIFY_FLEX_END;
    } else if (value.compare("space-between") == 0) {
        return CSSFLEX_JUSTIFY_SPACE_BETWEEN;
    } else if (value.compare("space-around") == 0) {
        return CSSFLEX_JUSTIFY_SPACE_AROUND;
    }
    return CSSFLEX_JUSTIFY_FLEX_START;
}

CSSStyleType ToFlexWrapType(const std::string& value) {
    if (value.compare("wrap") == 0) {
        return CSSFLEX_WRAP;
    } else if (value.compare("nowrap") == 0) {
        return CSSFLEX_NOWRAP;
    }
    return CSSFLEX_NOWRAP;
}

CSSStyleType ToPositionType(const std::string& value) {
    if (value.compare("absolute") == 0) {
        return CSS_POSITION_ABSOLUTE;
    } else if (value.compare("relative") == 0) {
        return CSS_POSITION_RELATIVE;
    } else if (value.compare("fixed") == 0) {
        return CSS_POSITION_FIXED;
    }
    return CSS_POSITION_RELATIVE;
}

TextStyleType ToTextAlignType(const std::string& value) {
    if (value.compare("left") == 0) {
        return CSSTEXT_ALIGN_LEFT;
    } else if (value.compare("right") == 0) {
        return CSSTEXT_ALIGN_RIGHT;
    } else if (value.compare("center") == 0) {
        return CSSTEXT_ALIGN_CENTER;
    }
    return CSSTEXT_ALIGN_LEFT;
}

TextStyleType ToTextDecorationType(const std::string& value) {
    if (value.compare("line-through") == 0) {
        return CSSTEXT_TEXTDECORATION_LINETHROUGH;
    }
    return CSSTEXT_TEXTDECORATION_NONE;
}

TextStyleType ToTextFontWeightType(const std::string& value) {
    if (value.compare("normal") == 0) {
        return CSSTEXT_FONT_WEIGHT_NORMAL;
    } else if (value.compare("bold") == 0) {
        return CSSTEXT_FONT_WEIGHT_BOLD;
    }
    return CSSTEXT_FONT_WEIGHT_NORMAL;
}

TextStyleType ToTextOverflowType(const std::string& value) {
    if (value.compare("ellipsis") == 0) {
        return CSSTEXT_OVERFLOW_ELLIPSIS;
    }
    return CSSTEXT_OVERFLOW_NONE;
}

TextStyleType ToTextWhiteSpaceType(const std::string& value) {
    if (value.compare("nowrap") == 0) {
        return CSSTEXT_WHITESPACE_NOWRAP;
    }
    return CSSTEXT_WHITESPACE_NORMAL;
}

ImageStyleType ToObjectFitType(const std::string& value) {
    if (value.compare("fill") == 0) {
        return CSSIMAGE_OBJECT_FIT_FILL;
    } else if (value.compare("contain") == 0) {
        return CSSIMAGE_OBJECT_FIT_CONTAIN;
    } else if (value.compare("cover") == 0) {
        return CSSIMAGE_OBJECT_FIT_COVER;
    }
    return CSSIMAGE_OBJECT_FIT_COVER;
}
}  // namesapce lynx
