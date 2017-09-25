// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_PARSER_RENDER_PARSER_H_
#define LYNX_PARSER_RENDER_PARSER_H_

#include <string>

#include "parser/input_stream.h"
#include "parser/render_tokenizer.h"
#include "parser/tree_builder.h"

namespace parser {
    class RenderParser {
    public:
        RenderParser(lynx::RenderTreeHost* tree_host,
                     jscore::ThreadManager* thread_manager) :
            input_(),
            tokenizer_(),
            tree_builder_(tree_host, thread_manager),
            token_() {}
        
        ~RenderParser() {}
        void Insert(const std::string& source);
    private:
        void PumpTokenizer();
        
        InputStream input_;
        RenderTokenizer tokenizer_;
        TreeBuilder tree_builder_;
        RenderToken token_;
    };
}

#endif  // LYNX_PARSER_RENDER_PARSER_H_
