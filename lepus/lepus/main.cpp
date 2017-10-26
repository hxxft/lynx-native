//
//  main.m
//  lepus
//
//  Created by dli on 2017/10/18.
//  Copyright © 2017年 dli. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

#include "parser/input_stream.h"
#include "lepus/scanner.h"
#include "lepus/parser.h"
#include "lepus/exception.h"
#include "lepus/semantic_analysis.h"
#include "lepus/interpreter.h"
#include "lepus/value.h"

int main(int argc, const char * argv[]) {
    std::ifstream t("../../test.js");
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    
    parser::InputStream input;
    input.Write(str);
    lepus::Scanner scanner(&input);
    lepus::Parser parser(&scanner);
    lepus::SemanticAnalysis semantic_analysis;
    lepus::Interpreter interpreter;
    lepus::ASTree* root = nullptr;
    try {
        root = parser.Parse();
        root->Accept(&interpreter, nullptr);
    }catch(const lepus::Exception& exception) {
        std::cout<<exception.message()<<std::endl;
    }
    interpreter.Call("onDispatchScrollEvent", std::vector<lepus::Value>());
    interpreter.Call("onScrollEvent1", std::vector<lepus::Value>());
    std::cout<<"hello lepus"<<std::endl;
    return 0;
}
