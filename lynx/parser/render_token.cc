// Copyright 2017 The Lynx Authors. All rights reserved.

#include "parser/render_token.h"

namespace parser {
    void RenderToken::BeginStartTag(char ch) {
        type_ = START_TAG;
        tag_name_.append(1, ch);
    }
}
