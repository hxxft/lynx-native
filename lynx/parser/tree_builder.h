// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_PARSER_TREE_BUILDER_H_
#define LYNX_PARSER_TREE_BUILDER_H_

#include <stack>

#include "render/render_object.h"
#include "parser/render_token.h"
#include "parser/style_parser.h"
#include "render/render_tree_host.h"

namespace parser {
    class TreeBuilder {
    public:
        TreeBuilder(lynx::RenderTreeHost* tree_host,
                    jscore::Runtime* runtime) :
                tree_host_(tree_host),
                runtime_(runtime){
            
        }
        
        lynx::RenderObject* tree() {
            return tree_host_->render_root();
        }
        
        void BuildTree(RenderToken& token);
    private:
        void BuildWithStartTag(RenderToken& token);
        void BuildWithEndTag(RenderToken& token);
        void BuildWithCharacter(RenderToken& token);
        
        void ProcessStartToken(RenderToken& token, lynx::RenderObject** renderer);
        
        void ApplyStyle(lynx::RenderObject* renderer,
                        const std::vector<std::string*>& styles);
        void ApplyAttribute(lynx::RenderObject* renderer,
                            const std::vector<RenderToken::Attribute*>& attribites);
        
        
        lynx::RenderTreeHost* tree_host_;
        jscore::Runtime* runtime_;
        std::stack<lynx::RenderObject*> renderer_stack_;
        StyleParser style_parser_;
        
    };
}

#endif  // LYNX_PARSER_TREE_BUILDER_H_
