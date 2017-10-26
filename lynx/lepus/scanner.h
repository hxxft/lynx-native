//
//  lexer.hpp
//  lynx
//
//  Created by dli on 2017/10/15.
//  Copyright © 2017年 lynx. All rights reserved.
//

#ifndef LYNX_LEPUS_LEXER_H_
#define LYNX_LEPUS_LEXER_H_

#include "base/scoped_ptr.h"
#include "lepus/token.h"
#include "parser/input_stream.h"

namespace lepus {
    class Scanner {
    public:
        Scanner(parser::InputStream* input);
        void NextToken(Token& token);
    private:
        void ParseNewLine();
        void ParseSingleLineComment();
        void ParseMultiLineComment();
        void ParseNumber(Token& token);
        void ParseEqual(Token& token, int equal);
        void ParseId(Token& token);
        
        int NextCharacter() {
            int character = input_stream_->Next();
            if(character != 0) {
                ++column_;
            }else{
                character = EOF;
            }
            return character;
        }
        
        parser::InputStream* input_stream_;
        int current_character_;
        int line_;
        int column_;
    };
}

#endif  // LYNX_LEPUS_LEXER_H_
