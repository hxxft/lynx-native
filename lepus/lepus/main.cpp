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

#include "lepus/interpreter_context.h"
#include "lepus/vm_context.h"
#include "lepus/vm.h"
#include "lepus/value.h"

int main(int argc, const char * argv[]) {
    std::ifstream t("../../test.js");
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    
    lepus::VM vm;
    lepus::VMContext ctx;
    ctx.Initialize();
    vm.Execute(&ctx, str);
    //vm.Call(&ctx, "onDispatchScrollEvent", std::vector<lepus::Value>());
    //vm.Call(&ctx, "onScrollEvent1", std::vector<lepus::Value>());
    std::cout<<"hello lepus"<<std::endl;
    return 0;
}
