// Copyright 2017 The Lynx Authors. All rights reserved.

#include "parser/render_tokenizer.h"
#include "parser/utils.h"

namespace parser {
    
    RenderTokenizer::RenderTokenizer() : state_(DATA_STATE) {
        
    }
    
    RenderTokenizer::~RenderTokenizer() {
        
    }
    
    bool RenderTokenizer::NextToken(InputStream& stream, RenderToken& token) {
        while(stream.HasNext()) {
            char cc = stream.Next();
            switch (state_) {
                case DATA_STATE:
                    if(cc == '<') {
                        if(token.type() == RenderToken::CHARACTER) {
                            stream.Back();
                            return true;
                        }
                        state_ = TAG_OPEN_STATE;
                    } else if(cc == '&') {
                        
                    }else {
                        token.type() = RenderToken::CHARACTER;
                        token.data().append(1, cc);
                    }
                    break;
                    
                case TAG_OPEN_STATE:
                    if(cc == '/') {
                        state_ = END_TAG_OPEN_STATE;
                    }else if(IsASCIILower(cc)) {
                        token.type() = RenderToken::START_TAG;
                        token.tag_name().append(1, cc);
                        state_ = TAG_NAME_STATE;
                    }else if(IsASCIIUpper(cc)) {
                        token.type() = RenderToken::START_TAG;
                        token.tag_name().append(1, cc);
                        state_ = TAG_NAME_STATE;
                    }
                    break;
                    
                case END_TAG_OPEN_STATE:
                    if (IsASCIIUpper(cc)) {
                        token.type() = RenderToken::END_TAG;
                        token.tag_name().append(1, ToLowerCase(cc));
                        state_ = TAG_NAME_STATE;
                    } else if (IsASCIILower(cc)) {
                        token.type() = RenderToken::END_TAG;
                        token.tag_name().append(1, cc);
                        state_ = TAG_NAME_STATE;
                    } else if (cc == '>') {
                        //parseError;
                        state_ = TAG_OPEN_STATE;
                    } else {
                        //parseError;
                    }
                    break;
                    
                case TAG_NAME_STATE:
                    if(IsWhitespace(cc)) {
                        state_ = BEFORE_ATTR_NAME_STATE;
                    }else if(cc == '/') {
                        state_ = SELF_CLOSING_START_TAG_STATE;
                    }else if(cc == '>'){
                        state_ = DATA_STATE;
                        return true;
                    }else if(IsASCIIUpper(cc)) {
                        token.tag_name().append(1, ToLowerCase(cc));
                        state_ = TAG_NAME_STATE;
                    }else {
                        token.tag_name().append(1, cc);
                        state_ = TAG_NAME_STATE;
                    }
                    break;
                    
                case BEFORE_ATTR_NAME_STATE:
                    if(IsWhitespace(cc)) {
                        state_ = BEFORE_ATTR_NAME_STATE;
                    }else if (cc == '/') {
                        state_ = SELF_CLOSING_START_TAG_STATE;
                    }else if (cc == '>'){
                        state_ = DATA_STATE;
                        return true;
                    }else if(IsASCIIUpper(cc)) {
                        token.NewAttribute();
                        token.attribute()->name_.append(1, ToLowerCase(cc));
                        state_ = ATTR_NAME_STATE;
                    }else {
                        token.NewAttribute();
                        token.attribute()->name_.append(1, cc);
                        state_ = ATTR_NAME_STATE;
                    }
                    break;
                    
                case ATTR_NAME_STATE:
                    if (IsWhitespace(cc)) {
                        state_ = AFTER_ATTR_NAME_STATE;
                    } else if (cc == '/') {
                        state_ = SELF_CLOSING_START_TAG_STATE;
                        //to-do
                    } else if (cc == '=') {
                        state_ = BEFORE_ATTR_VALUE_STATE;
                    } else if (cc == '>') {
                        state_ = DATA_STATE;
                        return true;
                    } else if (IsASCIIUpper(cc)) {
                        token.attribute()->name_.append(1, ToLowerCase(cc));
                        state_ = ATTR_NAME_STATE;
                    } else {
                        token.attribute()->name_.append(1, cc);
                        state_ = ATTR_NAME_STATE;
                    }
                    break;
                    
                case AFTER_ATTR_NAME_STATE:
                    break;
                    
                case BEFORE_ATTR_VALUE_STATE:
                    if (IsWhitespace(cc))
                        state_ = BEFORE_ATTR_VALUE_STATE;
                    else if (cc == '"') {
                        state_ = ATTR_VALUE_DOUBLE_QUOTED_STATE;
                    } else if (cc == '&') {
                       
                    } else if (cc == '\'') {
                        
                    } else if (cc == '>') {
                        //parseError;
                        state_ = DATA_STATE;
                        return true;
                    } else {
                        //Unknow
                    }
                    break;
                    
                case ATTR_VALUE_DOUBLE_QUOTED_STATE:
                    if (cc == '"') {
                        state_ = AFTER_ATTR_VALUE_DOUBLE_QUOTED_STATE;
                    } else if (cc == '&') {
                        //Unknow
                    } else {
                        token.attribute()->value_.append(1, cc);
                        state_ = ATTR_VALUE_DOUBLE_QUOTED_STATE;
                    }
                    break;
                case AFTER_ATTR_VALUE_DOUBLE_QUOTED_STATE:
                    if (IsWhitespace(cc))
                        state_ = BEFORE_ATTR_NAME_STATE;
                    else if (cc == '/')
                        state_ = SELF_CLOSING_START_TAG_STATE;
                    else if (cc == '>') {
                        state_ = DATA_STATE;
                        return true;
                    }else {
                        //parseError;
                        state_ = BEFORE_ATTR_VALUE_STATE;
                    }
                    break;
                    
                case SELF_CLOSING_START_TAG_STATE:
                    if (cc == '>') {
                        token.SetSelfClosing();
                        state_ = DATA_STATE;
                        return true;
                    } else {
                        //parseError;
                        state_ = BEFORE_ATTR_VALUE_STATE;
                    }
                    break;
                default:
                    break;
            }
        }
        return false;
    }
}
