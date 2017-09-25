// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_PARSER_RENDER_STYLE_H_
#define LYNX_PARSER_RENDER_STYLE_H_

#include <string>
#include "base/scoped_vector.h"'
#include "base/debug/memory_debug.h"

namespace parser {
    class RenderStyle {
    public:
        struct Style {
            std::string name_;
            std::string value_;
        };
        
        base::ScopedVector<Style>& styles() {
            return styles_;
        }
        
        Style& style() {
            return *current_style_;
        }
        
        void NewStyle() {
            current_style_ = lynx_new Style();
            styles_.push_back(current_style_);
        }
        
    private:
        base::ScopedVector<Style> styles_;
        Style* current_style_;
    };
}

#endif  // LYNX_PARSER_RENDER_STYLE_H_
