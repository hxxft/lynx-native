//
//  Token.hpp
//  lynx
//
//  Created by dli on 2017/10/15.
//  Copyright © 2017年 lynx. All rights reserved.
//

#ifndef LYNX_LEPUS_TOKEN_H_
#define LYNX_LEPUS_TOKEN_H_

#include <iostream>
#include "lepus/string.h"

namespace lepus {
    enum TokenType {
        Token_And       = 256,
        Token_Break     = 257,
        Token_Do        = 258,
        Token_Else      = 259,
        Token_Elseif    = 260,
        Token_End       = 261,
        Token_False     = 262,
        Token_For       = 263,
        Token_Function  = 264,
        Token_If        = 265,
        Token_In        = 266,
        Token_Var       = 267,
        Token_Nil       = 268,
        Token_Not       = 269,
        Token_Or        = 270,
        Token_Repeat    = 271,
        Token_Return    = 272,
        Token_Then      = 273,
        Token_True      = 274,
        Token_Until     = 275,
        Token_While     = 276,
        Token_Id        = 277,
        Token_String    = 278,
        Token_Number    = 279,
        Token_Equal     = 280,
        Token_NotEqual  = 281,
        Token_LessEqual = 282,
        Token_GreaterEqual = 283,
        Token_INC       = 284,  // ++
        Token_DEC       = 285,  // --
        Token_ASSIGN_BIT_OR = 286, // |=
        Token_ASSIGN_BIT_XOR = 287, // ^=
        Token_ASSIGN_BIT_AND = 288, // &=
        Token_ASSIGN_SHL = 289,      // <<=
        Token_ASSIGN_SAR = 290,      // >>>=
        Token_ASSIGN_SHR = 291,      // >>>=
        Token_ASSIGN_ADD = 292,      // +=
        Token_ASSIGN_SUB = 293,      // -=
        Token_ASSIGN_MUL = 294,      // *=
        Token_ASSIGN_DIV = 295,      // /=
        Token_ASSIGN_MOD = 296,      // %=
        Token_EOF          = 297,
    };
    
    struct Token {
        union {
            double number_;
            String* str_;
        };
        
        char* module_;
        int line_;
        int column_;
        int token_; // id & some character
        
        Token() :str_(nullptr), module_(nullptr), line_(0), column_(0), token_(Token_EOF) {
            
        }
        
        Token(Token& token) {
            str_ = nullptr;
            if(token.token_ == Token_Number){
                number_ = token.number_;
            }else if(token.str_){
                str_ = token.str_;
                str_->AddRef();
            }
            module_ = nullptr;
            token_ = token.token_;
            line_ = token.line_;
            column_ = token.column_;
        }
        
        Token(const Token& token) {
            str_ = nullptr;
            if(token.token_ == Token_Number){
                number_ = token.number_;
            }else if(token.str_){
                str_ = token.str_;
                str_->AddRef();
            }
            module_ = nullptr;
            token_ = token.token_;
            line_ = token.line_;
            column_ = token.column_;
        }
        
        ~Token() {
            if(token_ != Token_Number && token_ != Token_EOF && str_) {
                str_->Release();
            }
        }
        
        void Print(){
            if(token_ < Token_And) {
                printf("%c", (char)token_);
            }
            else if(token_ == Token_Number) {
                printf("%lf", number_);
            }else if(token_ != Token_EOF){
                printf("%s", str_->c_str());
            }
        }
    };
}

#endif  // LYNX_LEPUS_TOKEN_H_
