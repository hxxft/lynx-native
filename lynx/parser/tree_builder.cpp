// Copyright 2017 The Lynx Authors. All rights reserved.

#include "parser/tree_builder.h"
#include "parser/lynx_names.h"
#include "render/render_factory.h"
#include "runtime/runtime.h"

namespace parser {
    void TreeBuilder::BuildTree(RenderToken& token) {
        switch(token.type()) {
            case RenderToken::START_TAG:
                BuildWithStartTag(token);
                break;
               
            case RenderToken::END_TAG:
                BuildWithEndTag(token);
                break;
                
            case RenderToken::CHARACTER:
                BuildWithCharacter(token);
                break;
                
            default:
                break;

          }
    }

    void TreeBuilder::ProcessStartToken(RenderToken& token, lynx::RenderObject** renderer) {

        std::vector<std::string*> styles;
        std::vector<RenderToken::Attribute*> attributes;
        std::string id;

        for(int i = 0; i < token.attributes().size(); ++i) {
            RenderToken::Attribute* attr = token.attributes()[i];
            if(attr->name_ == tag_attr && attr->value_ != body_tag) {
                *renderer = lynx::RenderFactory::CreateRenderObject(
                        runtime_->thread_manager(), attr->value_, tree_host_);
            }else if(attr->name_ == style_attr) {
                styles.push_back(&attr->value_);
            }else if(attr->name_ == id_attr) {
                id = attr->value_;
            }else {
                attributes.push_back(attr);
            }
        }
        if(!id.empty()) {
            tree_host_->SetElementId(*renderer, id);
        }

        ApplyStyle(*renderer, styles);
        ApplyAttribute(*renderer, attributes);

        return;
    }
    
    void TreeBuilder::BuildWithStartTag(RenderToken& token) {

        lynx::RenderObject* renderer = NULL;

        if(token.tag_name() == body_tag) {
            renderer = lynx::RenderFactory::CreateRenderObject(
                    runtime_->thread_manager(), token.tag_name(), tree_host_);

            lynx::RenderObject* body = tree_host_->render_root();
            ProcessStartToken(token, &body);

            renderer_stack_.push(renderer);
            return;
        }

        if(token.tag_name() == div_tag ||
                token.tag_name() == img_tag){

            ProcessStartToken(token, &renderer);

            lynx::RenderObject* parent = renderer_stack_.empty() ? NULL : renderer_stack_.top();

            if(!token.is_self_closing() && token.tag_name() != img_tag) {
                renderer_stack_.push(renderer);
            }

            if(parent) {
                parent->AppendChild(renderer);
            }
            return;
        }

        if(token.tag_name() == script_tag) {
            for(int i = 0; i < token.attributes().size(); ++i) {
                RenderToken::Attribute* attr = token.attributes()[i];
                if(attr->name_ == src_attr) {
                    std::string url = attr->value_;
                    runtime_->LoadScript(url,loader::LynxLoader::SCRIPT_FILE);
                }
            }
            return;
        }
    }
    
    void TreeBuilder::BuildWithEndTag(RenderToken& token) {
        if(!renderer_stack_.empty() &&
                (token.tag_name() == body_tag ||
                token.tag_name() == img_tag ||
                token.tag_name() == div_tag)){
            renderer_stack_.pop();
        }
    }
    
    void TreeBuilder::BuildWithCharacter(RenderToken& token) {
        if(renderer_stack_.empty()) return;
        lynx::RenderObject* renderer = renderer_stack_.top();
        lynx::RenderObject* text_node = lynx::RenderFactory::CreateRenderObject(
                runtime_->thread_manager(), "text", tree_host_);
        if(renderer &&  text_node
           && renderer->render_object_type()
              == lynx::RenderObjectType::LYNX_LABEL) {
            renderer->AppendChild(text_node);
            text_node->SetText(token.data());
        }
    }

    
    void TreeBuilder::ApplyStyle(lynx::RenderObject* renderer,
                                 const std::vector<std::string*>& styles) {
        size_t size = styles.size();
        for(int i = 0; i < size; ++i) {
            style_parser_.Parse(*styles[i]);
            style_parser_.Apply(renderer);
        }
    }
    
    void TreeBuilder::ApplyAttribute(lynx::RenderObject* renderer,
                                 const std::vector<RenderToken::Attribute*>& attributes) {
        size_t size = attributes.size();
        for(int i = 0; i < size; ++i) {
            renderer->SetAttribute(attributes[i]->name_, attributes[i]->value_);
        }
    }
}
