// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_PARSER_RENDER_TOKENIZER_H_
#define LYNX_PARSER_RENDER_TOKENIZER_H_

#include <string>
#include "parser/render_token.h"
#include "parser/input_stream.h"

namespace parser {
    class RenderTokenizer {
    public:
        RenderTokenizer();
        ~RenderTokenizer();
        bool NextToken(InputStream& stream, RenderToken& token);
    private:
        enum State{
            DATA_STATE,
            TAG_OPEN_STATE,
            END_TAG_OPEN_STATE,
            TAG_NAME_STATE,
            BEFORE_ATTR_NAME_STATE,
            ATTR_NAME_STATE,
            AFTER_ATTR_NAME_STATE,
            BEFORE_ATTR_VALUE_STATE,
            ATTR_VALUE_DOUBLE_QUOTED_STATE,
            AFTER_ATTR_VALUE_DOUBLE_QUOTED_STATE,
            SELF_CLOSING_START_TAG_STATE,
        };
        
        State state_;
        
    };
}

#endif  // LYNX_PARSER_RENDER_TOKENIZER_H_
