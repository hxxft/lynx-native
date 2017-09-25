// Copyright 2017 The Lynx Authors. All rights reserved.

#include "parser/input_stream.h"

namespace parser {
    InputStream::InputStream() : cursor_(0), source_() {
        
    }
    
    InputStream::~InputStream() {
        
    }
    
    void InputStream::Write(const std::string& source) {
        source_.append(source);
    }
    
    bool InputStream::HasNext() {
        return source_[cursor_] != 0;
    }
    
    char InputStream::Next() {
        return HasNext() ? source_[cursor_++] : 0;
    }
    
    void InputStream::Back() {
        --cursor_;
    }
}
