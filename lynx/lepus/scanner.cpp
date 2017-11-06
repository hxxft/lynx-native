//
//  lexer.cpp
//  lynx
//
//  Created by dli on 2017/10/15.
//  Copyright © 2017年 lynx. All rights reserved.
//

#include "lepus/scanner.h"

#include "lepus/utils.h"
#include "lepus/string.h"

namespace lepus {
    Scanner::Scanner(parser::InputStream* input, StringPool* string_pool)
        : input_stream_(input),
        string_pool_(string_pool),
        current_character_(EOF),
        line_(0),
        column_(0){
        
    }
    
    void Scanner::ParseNewLine() {
        int next = NextCharacter();
        if(IsNewLine(next) && next != current_character_) {
            current_character_ = NextCharacter();
        } else {
            current_character_ = next;
        }
        ++line_;
        column_ = 0;
    }
    
    void Scanner::ParseSingleLineComment() {
        while(!IsNewLine(current_character_) && current_character_ != EOF) {
            current_character_ = NextCharacter();
        }
    }

    void Scanner::ParseMultiLineComment() {
        while(true) {
            current_character_ = NextCharacter();
            if(current_character_ == '*') {
                int next = NextCharacter();
                if(next == '/') {
                    current_character_ = NextCharacter();
                    break;
                }else{
                    current_character_ = next;
                }
            }else if(IsNewLine(current_character_)) {
                ParseNewLine();
            }else if(current_character_ == EOF) {
                // TODO Error
            }
        }
    }
    
    void Scanner::ParseNumber(Token& token) {
        std::string buffer;
        while(true) {
            buffer.push_back(current_character_);
            current_character_ = NextCharacter();
            if(!IsDigit(current_character_)
               && !IsHex(current_character_)
               && current_character_ != '.') {
                break;
            }
        }
       
        token.token_ = Token_Number;
        token.column_ = column_;
        token.line_ = line_;
        token.number_ = strtod(buffer.c_str(), nullptr);
    }
    
    void Scanner::ParseEqual(Token& token, int equal) {
        int next = NextCharacter();
        if(next != '=') {
            token.line_ = line_;
            token.column_ = column_;
            token.token_ = current_character_;
            current_character_ = next;
        }else{
            token.line_ = line_;
            token.column_ = column_;
            token.token_ = equal;
            current_character_ = NextCharacter();
        }
    }
    
    void Scanner::ParseTokenCharacter(Token& token, int token_character) {
        if(token_character < Token_And) {
            token.line_ = line_;
            token.column_ = column_;
            token.token_ = current_character_;
            current_character_ = token_character;
        }else{
            token.line_ = line_;
            token.column_ = column_;
            token.token_ = token_character;
            current_character_ = NextCharacter();
        }
    }
    
    void Scanner::ParseId(Token& token) {
        if(!isalpha(current_character_) &&
           current_character_ != '_') {
            // TODO
        }
        std::string buffer;
        buffer.push_back(current_character_);
        while((current_character_ = NextCharacter()) && (isalnum(current_character_) || current_character_ == '_')) {
            buffer.push_back(current_character_);
        }
        
        if(!IsKeyWord(buffer, token.token_)) {
            token.token_ = Token_Id;
        }
        token.line_  = line_;
        token.column_ = column_;
        token.str_ = string_pool_->NewString(buffer);
    }
    
    void Scanner::NextToken(Token& token) {
        if(current_character_ == EOF) {
            current_character_ = NextCharacter();
        }
        
        while(current_character_ != EOF) {
            if(IsWhitespace(current_character_)) {
                current_character_ = NextCharacter();
                
            }else if(IsNewLine(current_character_)) {
                ParseNewLine();
            }else if(IsDigit(current_character_)){
                return ParseNumber(token);
            }else if(current_character_ == '/') {
                int next = NextCharacter();
                if(next == '/') {
                    ParseSingleLineComment();
                }else if(next == '*'){
                    ParseMultiLineComment();
                }else if(next == '='){
                    return ParseTokenCharacter(token, Token_ASSIGN_DIV);
                }else{
                    return ParseTokenCharacter(token, next);
                }
            }else if(current_character_ == '+') {
                int next = NextCharacter();
                if(next == '+') {
                    return ParseTokenCharacter(token, Token_INC);
                }else if(next == '=') {
                    return ParseTokenCharacter(token, Token_ASSIGN_ADD);
                }else{
                    return ParseTokenCharacter(token, next);
                }
            }else if(current_character_ == '-') {
                int next = NextCharacter();
                if(next == '-') {
                    return ParseTokenCharacter(token, Token_DEC);
                }else if(next == '=') {
                    return ParseTokenCharacter(token, Token_ASSIGN_SUB);
                }else{
                    return ParseTokenCharacter(token, next);
                }
            }else if(current_character_ == '*') {
                int next = NextCharacter();
                 if(next == '=') {
                    return ParseTokenCharacter(token, Token_ASSIGN_MUL);
                }else{
                    return ParseTokenCharacter(token, next);
                }
            }else if(current_character_ == '%') {
                int next = NextCharacter();
                if(next == '=') {
                    return ParseTokenCharacter(token, Token_ASSIGN_MUL);
                }else{
                    return ParseTokenCharacter(token, next);
                }
            }else if(current_character_ == '^') {
                int next = NextCharacter();
                if(next == '=') {
                    return ParseTokenCharacter(token, Token_ASSIGN_BIT_XOR);
                }else{
                    return ParseTokenCharacter(token, next);
                }
            }else if(current_character_ == '|') {
                int next = NextCharacter();
                if(next == '=') {
                    return ParseTokenCharacter(token, Token_ASSIGN_BIT_OR);
                }else if(next == '|') {
                    return ParseTokenCharacter(token, Token_Or);
                }else{
                    return ParseTokenCharacter(token, next);
                }
            }else if(current_character_ == '&') {
                int next = NextCharacter();
                if(next == '=') {
                    return ParseTokenCharacter(token, Token_ASSIGN_BIT_AND);
                }else if(next == '&') {
                    return ParseTokenCharacter(token, Token_And);
                }else{
                    return ParseTokenCharacter(token, next);
                }
            }else if(IsOtherToken(current_character_)){
               return ParseTokenCharacter(token, NextCharacter());
            }else if(current_character_ == '!') {
                return ParseEqual(token, Token_NotEqual);
            }else if(current_character_ == '=') {
                return ParseEqual(token, Token_Equal);
            }else if(current_character_ == '>') {
                return ParseEqual(token, Token_GreaterEqual);
            }else if(current_character_ == '<') {
                return ParseEqual(token, Token_LessEqual);
            }else if(current_character_ == '"') {
                
            }else {
                return ParseId(token);
            }
        }
        token.token_ = Token_EOF;
    }
    
}
