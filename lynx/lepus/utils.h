//
//  utils.h
//  lynx
//
//  Created by dli on 2017/10/15.
//  Copyright © 2017年 lynx. All rights reserved.
//

#ifndef LYNX_LEPUS_UTILS_H_
#define LYNX_LEPUS_UTILS_H_

#include "lepus/token.h"

namespace lepus {
    static inline bool IsWhitespace(int c) {
        return c == ' ' || c == '\t'
        || c == '\v' || c == '\f';
    }
    
    static inline bool IsNewLine(int c) {
        return c == '\r' || c == '\n';
    }
    
    static inline bool IsDigit(int c) {
        return c >= '0' && c <= '9';
    }
    
    static inline bool IsHex(int c) {
        return (c >= '0' && c <= '9') ||
        (c >= 'a' && c <= 'f') ||
        (c >= 'A' && c <= 'F');
    }
    
    static inline bool IsOtherToken(int c) {
        return c == '#' || c == '(' || c == ')'
        || c == '{' || c == '}' || c == ']' || c == ']'
        || c == ';' || c == ':' || c == ',' || c == '.';
    }
    
    const static std::map<std::string, int> kKeyWords = {
        {"break", Token_Break},
        {"do", Token_Do},
        {"if", Token_If},
        {"else", Token_Else},
        {"elseif", Token_Elseif},
        {"false", Token_False},
        {"true", Token_True},
        {"function", Token_Function},
        {"for", Token_For},
        {"var", Token_Var},
        {"null", Token_Nil},
        {"while", Token_While}
    };
    static inline bool IsKeyWord(const std::string& word, int& token) {
        auto iter = kKeyWords.find(word);
        if(iter != kKeyWords.end()) {
            token = iter->second;
            return true;
        }
        return false;
    }
    
    static inline bool IsPrimaryExpr(int token) {
        return
        token == Token_Nil ||
        token == Token_False ||
        token == Token_True ||
        token == Token_Number ||
        token == Token_String ||
        token == Token_Function ||
        token == Token_Id ||
        token == '(' ||
        token == '{';
    }
}



#endif  // LYNX_LEPUS_UTILS_H_
