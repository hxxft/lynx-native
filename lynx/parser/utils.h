// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_PARSER_UTILS_H_
#define LYNX_PARSER_UTILS_H_

namespace parser {
    static bool IsASCIILower(const char& ch) {
        return ch >= 'a' && ch <= 'z';
    }
    
    static bool IsASCIIUpper(const char& ch) {
        return ch >= 'A' && ch <= 'Z';
    }
    
    static bool IsWhitespace(char ch) {
        return ch == ' ' || ch == '\x0A' || ch == '\x09' || ch == '\x0C';
    }
    
    static char ToLowerCase(char ch) {
        const int lowerCaseOffset = 0x20;
        return ch + lowerCaseOffset;
    }
}

#endif  // LYNX_PARSER_UTILS_H_
