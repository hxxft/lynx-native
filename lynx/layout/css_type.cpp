// Copyright 2017 The Lynx Authors. All rights reserved.

#include "layout/css_type.h"

namespace lynx {

#define TRUE_RETURN(result) \
  do {                      \
    type = result;          \
    return true;            \
  } while (0);

#define FALSE_RETURN(result) \
  do {                       \
    type = result;           \
    return false;            \
  } while (0);

bool ToVisibleType(const std::string& value, CSSStyleType& type) {
  if (value.compare("visible") == 0) {
    TRUE_RETURN(CSS_VISIBLE);
  } else if (value.compare("hidden") == 0) {
    TRUE_RETURN(CSS_HIDDEN);
  }
  FALSE_RETURN(CSS_VISIBLE);
}

bool ToDisplayType(const std::string& value, CSSStyleType& type) {
  if (value.compare("none") == 0) {
    TRUE_RETURN(CSS_DISPLAY_NONE);
  } else if (value.compare("flex") == 0) {
    TRUE_RETURN(CSS_DISPLAY_FLEX);
  }
  FALSE_RETURN(CSS_DISPLAY_FLEX);
}

bool ToFlexAlignType(const std::string& value, CSSStyleType& type) {
  if (value.compare("flex-start") == 0) {
    TRUE_RETURN(CSSFLEX_ALIGN_FLEX_START;);
  } else if (value.compare("flex-end") == 0) {
    TRUE_RETURN(CSSFLEX_ALIGN_FLEX_END);
  } else if (value.compare("center") == 0) {
    TRUE_RETURN(CSSFLEX_ALIGN_CENTER);
  } else if (value.compare("stretch") == 0) {
    TRUE_RETURN(CSSFLEX_ALIGN_STRETCH);
  }
  FALSE_RETURN(CSSFLEX_ALIGN_AUTO);
}

bool ToFlexDirectionType(const std::string& value, CSSStyleType& type) {
  if (value.compare("column") == 0) {
    TRUE_RETURN(CSSFLEX_DIRECTION_COLUMN);
  } else if (value.compare("row") == 0) {
    TRUE_RETURN(CSSFLEX_DIRECTION_ROW);
  }
  FALSE_RETURN(CSSFLEX_DIRECTION_COLUMN);
}

bool ToFlexJustifyType(const std::string& value, CSSStyleType& type) {
  if (value.compare("flex-start") == 0) {
    TRUE_RETURN(CSSFLEX_JUSTIFY_FLEX_START);
  } else if (value.compare("center") == 0) {
    TRUE_RETURN(CSSFLEX_JUSTIFY_FLEX_CENTER);
  } else if (value.compare("flex-end") == 0) {
    TRUE_RETURN(CSSFLEX_JUSTIFY_FLEX_END);
  } else if (value.compare("space-between") == 0) {
    TRUE_RETURN(CSSFLEX_JUSTIFY_SPACE_BETWEEN);
  } else if (value.compare("space-around") == 0) {
    TRUE_RETURN(CSSFLEX_JUSTIFY_SPACE_AROUND);
  }
  FALSE_RETURN(CSSFLEX_JUSTIFY_FLEX_START);
}

bool ToFlexWrapType(const std::string& value, CSSStyleType& type) {
  if (value.compare("wrap") == 0) {
    TRUE_RETURN(CSSFLEX_WRAP);
  } else if (value.compare("nowrap") == 0) {
    TRUE_RETURN(CSSFLEX_NOWRAP);
  }
  FALSE_RETURN(CSSFLEX_NOWRAP);
}

bool ToPositionType(const std::string& value, CSSStyleType& type) {
  if (value.compare("absolute") == 0) {
    TRUE_RETURN(CSS_POSITION_ABSOLUTE);
  } else if (value.compare("relative") == 0) {
    TRUE_RETURN(CSS_POSITION_RELATIVE);
  } else if (value.compare("fixed") == 0) {
    TRUE_RETURN(CSS_POSITION_FIXED);
  }
  FALSE_RETURN(CSS_POSITION_RELATIVE);
}

bool ToTextAlignType(const std::string& value, TextStyleType& type) {
  if (value.compare("left") == 0) {
    TRUE_RETURN(CSSTEXT_ALIGN_LEFT);
  } else if (value.compare("right") == 0) {
    TRUE_RETURN(CSSTEXT_ALIGN_RIGHT);
  } else if (value.compare("center") == 0) {
    TRUE_RETURN(CSSTEXT_ALIGN_CENTER);
  }
  FALSE_RETURN(CSSTEXT_ALIGN_LEFT);
}

bool ToTextDecorationType(const std::string& value, TextStyleType& type) {
  if (value.compare("line-through") == 0) {
    TRUE_RETURN(CSSTEXT_TEXTDECORATION_LINETHROUGH);
  }
  FALSE_RETURN(CSSTEXT_TEXTDECORATION_NONE);
}

bool ToTextFontWeightType(const std::string& value, TextStyleType& type) {
  if (value.compare("normal") == 0) {
    TRUE_RETURN(CSSTEXT_FONT_WEIGHT_NORMAL);
  } else if (value.compare("bold") == 0) {
    TRUE_RETURN(CSSTEXT_FONT_WEIGHT_BOLD);
  }
  FALSE_RETURN(CSSTEXT_FONT_WEIGHT_NORMAL);
}

bool ToTextOverflowType(const std::string& value, TextStyleType& type) {
  if (value.compare("ellipsis") == 0) {
    TRUE_RETURN(CSSTEXT_OVERFLOW_ELLIPSIS);
  }
  FALSE_RETURN(CSSTEXT_OVERFLOW_NONE);
}

bool ToTextWhiteSpaceType(const std::string& value, TextStyleType& type) {
  if (value.compare("nowrap") == 0) {
    TRUE_RETURN(CSSTEXT_WHITESPACE_NOWRAP);
  }
  FALSE_RETURN(CSSTEXT_WHITESPACE_NORMAL);
}

bool ToObjectFitType(const std::string& value, ImageStyleType& type) {
  if (value.compare("fill") == 0) {
    TRUE_RETURN(CSSIMAGE_OBJECT_FIT_FILL);
  } else if (value.compare("contain") == 0) {
    TRUE_RETURN(CSSIMAGE_OBJECT_FIT_CONTAIN);
  } else if (value.compare("cover") == 0) {
    TRUE_RETURN(CSSIMAGE_OBJECT_FIT_COVER);
  }
  FALSE_RETURN(CSSIMAGE_OBJECT_FIT_COVER);
}
}  // namesapce lynx
