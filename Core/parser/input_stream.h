// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_PARSER_INPUT_STREAM_H_
#define LYNX_PARSER_INPUT_STREAM_H_

#include <string>

namespace parser {
    class InputStream {
    public:
        InputStream();
        ~InputStream();
        void Write(const std::string& source);
        bool HasNext();
        char Next();
        void Back();
    private:
        int cursor_;
        std::string source_;
    };
}

#endif  // LYNX_PARSER_INPUT_STREAM_H_
