// Copyright 2017 The Lynx Authors. All rights reserved.

#include "parser/render_parser.h"

#include "base/trace_event/trace_event_common.h"

namespace parser {
    void RenderParser::Insert(const std::string& source) {
        input_.Write(source);
        PumpTokenizer();
    }
    
    void RenderParser::PumpTokenizer() {
        TRACE_EVENT0("parser", "RenderParser::PumpTokenizer");
        while(input_.HasNext()) {
            if(tokenizer_.NextToken(input_, token_)) {
                tree_builder_.BuildTree(token_);
                token_.Clear();
            }
        }
    }

}
