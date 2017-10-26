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
        Token_Concat       = 284,
        Token_VarArg       = 285,
        Token_EOF          = 286,
    };
    
    struct Token {
        union {
            double number_;
            std::string* str_;
        };
        
        char* module_;
        int line_;
        int column_;
        int token_; // id & some character
        
        Token() :str_(nullptr), module_(nullptr), line_(0), column_(0), token_(Token_EOF) {
            
        }
        
        void Print(){
            if(token_ < Token_And)
                std::cout << (char)token_ << std::endl;
            else if(token_ == Token_Number) {
                std::cout << number_ << std::endl;
            }else if(token_ != Token_EOF){
                std::cout << *str_ << std::endl;
            }
            
        }
    };
}

#endif  // LYNX_LEPUS_TOKEN_H_
