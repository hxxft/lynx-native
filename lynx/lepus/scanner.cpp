//
//  lexer.cpp
//  lynx
//
//  Created by dli on 2017/10/15.
//  Copyright © 2017年 lynx. All rights reserved.
//

#include "lepus/scanner.h"

#include "lepus/utils.h"
#include "lepus/lepus_string.h"
#include "lepus/exception.h"

namespace lepus {
    Scanner::Scanner(parser::InputStream* input, StringPool* string_pool)
        : input_stream_(input),
        string_pool_(string_pool),
        current_character_(EOF),
        line_(1),
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
                ++line_;
                column_ = 0;
            }else if(current_character_ == EOF) {
                // TODO Error
                break;
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
        token = Token(line_, column_, Token_Number, strtod(buffer.c_str(), nullptr));
    }
    
    void Scanner::ParseEqual(Token& token, int equal) {
        int next = NextCharacter();
        if(next != '=') {
            token = Token(line_, column_, current_character_);
            current_character_ = next;
        }else{
            int next_next = NextCharacter();
            if(next_next != '='){
                throw CompileException("only support === and !==", Token(line_, column_, equal));
            }
            token = Token(line_, column_, equal);
            current_character_ = NextCharacter();
        }
    }
    
    void Scanner::ParseTokenCharacter(Token& token, int token_character) {
        if(token_character < Token_And) {
            token = Token(line_, column_, current_character_);
            current_character_ = token_character;
        }else{
            token = Token(line_, column_, token_character);
            current_character_ = NextCharacter();
        }
    }
    
    void Scanner::ParseString(Token& token) {
        int quote = current_character_;
        std::string buffer;
        while((current_character_ = NextCharacter()) && (current_character_ != quote)) {
            buffer.push_back(current_character_);
        }
        current_character_ = NextCharacter();
        token = Token(line_, column_, Token_String, string_pool_->NewString(buffer));
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
        
        int token_type = Token_EOF;
        if(!IsKeyWord(buffer, token_type)) {
            token = Token(line_, column_, Token_Id, string_pool_->NewString(buffer));
            return;
        }
        token = Token(line_, column_, token_type);
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
            }else if(current_character_ == '"' || current_character_ == '\'') {
                return ParseString(token);
            }else {
                return ParseId(token);
            }
        }
        token = Token(line_, column_, Token_EOF);
    }
    
}
