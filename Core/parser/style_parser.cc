// Copyright 2017 The Lynx Authors. All rights reserved.

#include "parser/style_parser.h"

#include "parser/utils.h"
#include "render/render_object.h"

namespace parser {
    void StyleParser::Parse(const std::string& source) {
        
        state_ = DATA_STATE;
        style_.styles().clear();
        
        int cursor = 0;
        char ch;
        while((ch = source[cursor++]) &&  ch != 0) {
            switch (state_) {
                case DATA_STATE:
                    if(IsWhitespace(ch))
                        state_ = DATA_STATE;
                    else if(IsASCIILower(ch)) {
                        style_.NewStyle();
                        style_.style().name_.append(1, ch);
                        state_ = STYLE_NAME_STATE;
                    }else if(IsASCIIUpper(ch)) {
                        style_.NewStyle();
                        style_.style().name_.append(1, ToLowerCase(ch));
                        state_ = STYLE_NAME_STATE;
                    }else {
                        //error
                    }
                    break;
                    
                case STYLE_NAME_STATE:
                    if(IsWhitespace(ch) || ch == ':')
                        state_ = STYLE_VALUE_STATE;
                    else if(IsASCIIUpper(ch)) {

                        style_.style().name_.append(1, ToLowerCase(ch));
                        state_ = STYLE_NAME_STATE;
                    }else {
                        //error
                        style_.style().name_.append(1, ch);
                        state_ = STYLE_NAME_STATE;
                    }
                    break;
                    
                case STYLE_VALUE_STATE:
                    if(IsWhitespace(ch) && style_.style().value_.empty())
                        state_ = STYLE_VALUE_STATE;
                    else if(ch == ';')
                        state_ = DATA_STATE;
                    else if(IsASCIIUpper(ch)) {
                        style_.style().value_.append(1, ToLowerCase(ch));
                        state_ = STYLE_VALUE_STATE;
                    }else {
                        style_.style().value_.append(1, ch);
                        state_ = STYLE_VALUE_STATE;
                    }
                    break;
                    
                default:
                    break;
            }
        }
    }
    
    void StyleParser::Apply(lynx::RenderObject* renderer) {
        size_t style_count = style_.styles().size();
        for(int i = 0; i < style_count; ++i) {
            renderer->SetStyle(style_.styles()[i]->name_, style_.styles()[i]->value_);
        }
        renderer->FlushStyle();
    }
}
