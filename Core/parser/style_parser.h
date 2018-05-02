// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_PARSER_STYLE_PARSER_H_
#define LYNX_PARSER_STYLE_PARSER_H_

#include <string>

#include "parser/render_style.h"

namespace lynx {
    class RenderObject;
}

namespace parser {
    class StyleParser {
    public:
        enum State {
            DATA_STATE,
            STYLE_NAME_STATE,
            STYLE_VALUE_STATE,
        };
        
        void Parse(const std::string& source);
        void Apply(lynx::RenderObject* renderer);
        
    private:
        State state_;
        RenderStyle style_;
    };
}

#endif  // LYNX_PARSER_STYLE_PARSER_H_
