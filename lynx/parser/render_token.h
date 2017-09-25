// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_PASER_RENDER_TOKEN_H_
#define LYNX_PASER_RENDER_TOKEN_H_

#include <string>

#include "base/scoped_vector.h"
#include "base/scoped_ptr.h"
#include "base/debug/memory_debug.h"

namespace parser {
    class RenderToken {
    public:
        enum TokenType {
            UNINITIALIZED,
            START_TAG,
            END_TAG,
            CHARACTER,
        };

        struct StringRange {

            StringRange() : raw_(NULL), start_(0), end_(0) {

            }

            void Append(std::string* raw, int cursor) {
                if(raw_ == NULL) {
                    raw_ = raw;
                    start_ = cursor;
                }else{
                    end_ = cursor;
                }
            }

            std::string* raw_;
            int start_;
            int end_;
        };
        
        struct Attribute {
            std::string name_;
            std::string value_;
        };
        
        RenderToken():
            type_(UNINITIALIZED),
            data_(),
            tag_name_(),
            attributes_(),
            current_attribute_(NULL),
            is_self_closing_(false) {
            
        }
        
        ~RenderToken() {
            
        }
        
        TokenType& type() {
            return type_;
        }
        
        std::string& data() {
            return data_;
        }
        
        std::string& tag_name() {
            return tag_name_;
        }
        
        void BeginStartTag(char ch);
        
        void NewAttribute() {
            current_attribute_ = lynx_new Attribute();
            attributes_.push_back(current_attribute_);
        }
        
        Attribute* attribute() {
            return current_attribute_;
        }
        
        base::ScopedVector<Attribute>& attributes() {
            return attributes_;
        }
        
        void SetSelfClosing() {
            is_self_closing_ = true;
        }
        
        bool is_self_closing() {
            return is_self_closing_;
        }
        
        void Clear() {
            type_ = UNINITIALIZED;
            data_ = "";
            tag_name_ = "";
            attributes_.clear();
            current_attribute_ = NULL;
            is_self_closing_ = false;
        }
        
    private:
        TokenType type_;
        std::string data_;
        std::string tag_name_;
        base::ScopedVector<Attribute> attributes_;
        Attribute* current_attribute_;
        bool is_self_closing_;
    };
}

#endif  // LYNX_PASER_RENDER_TOKEN_H_
