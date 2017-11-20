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
#include "lepus/lepus_string.h"

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
        Token_Return    = 271,
        Token_Switch    = 272,
        Token_Case      = 273,
        Token_Defalut      = 274,
        Token_True      = 275,
        Token_Until     = 276,
        Token_While     = 277,
        Token_Id        = 278,
        Token_String    = 279,
        Token_Number    = 280,
        Token_Equal     = 281,
        Token_NotEqual  = 282,
        Token_LessEqual = 283,
        Token_GreaterEqual = 284,
        Token_INC       = 285,  // ++
        Token_DEC       = 286,  // --
        Token_ASSIGN_BIT_OR = 287, // |=
        Token_ASSIGN_BIT_XOR = 288, // ^=
        Token_ASSIGN_BIT_AND = 289, // &=
        Token_ASSIGN_SHL = 290,      // <<=
        Token_ASSIGN_SAR = 291,      // >>>=
        Token_ASSIGN_SHR = 292,      // >>>=
        Token_ASSIGN_ADD = 293,      // +=
        Token_ASSIGN_SUB = 294,      // -=
        Token_ASSIGN_MUL = 295,      // *=
        Token_ASSIGN_DIV = 296,      // /=
        Token_ASSIGN_MOD = 297,      // %=
        Token_EOF          = 298,
    };
    
    struct Token {
        union {
            double number_;
            String* str_;
        };
        
        String* module_;
        int line_;
        int column_;
        int token_; // id & some character
        
        Token() :str_(nullptr), module_(nullptr), line_(0), column_(0), token_(Token_EOF) {
            
        }
        
        Token(int line, int column, int token) : line_(line), column_(column), token_(token) {
            
        }
        
        Token(int line, int column, int token, double number)
            :number_(number),  line_(line), column_(column), token_(token) {
            
        }
        
        Token(int line, int column, int token, String* str)
            :str_(str), line_(line), column_(column), token_(token) {
                str->AddRef();
        }
        
        Token(const Token& token) {
            Copy(token);
        }
        
        ~Token() {
            if(IsString()) {
                str_->Release();
            }
        }
        
        Token& operator=(const Token& token) {
            if(IsString()) {
                str_->Release();
            }
            Copy(token);
            return *this;
        }
        
        bool inline IsString() const{
            return (token_ == Token_String || token_ == Token_Id) && str_;
        }
    private:
        void Copy(const Token& token) {
            str_ = nullptr;
            
            if(token.token_ == Token_Number){
                number_ = token.number_;
            }else if(token.IsString()){
                str_ = token.str_;
                str_->AddRef();
            }
            token_ = token.token_;
            line_ = token.line_;
            column_ = token.column_;
        }
    };
}

#endif  // LYNX_LEPUS_TOKEN_H_
