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

#include "lepus/vm_context.h"
#include "lepus/vm.h"
#include "lepus/value.h"

int main(int argc, const char * argv[]) {
    std::ifstream t("../../../test.js");
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    
    lepus::VM vm;
    lepus::VMContext ctx;
    ctx.Initialize();
    vm.Execute(&ctx, str);
    std::vector<lepus::Value> args;
    lepus::Value v1;
    v1.number_ = 3;
    v1.type_ = lepus::Value_Number;
    args.push_back(v1);
    lepus::Value v2;
    v2.number_ = 3;
    v2.type_ = lepus::Value_Number;
    args.push_back(v2);
//    vm.Call(&ctx, "onScrollEvent1", args);
//    vm.Call(&ctx, "onScrollEvent1", args);
    //vm.Call(&ctx, "onScrollEvent1", std::vector<lepus::Value>());
    std::cout<<"hello lepus"<<std::endl;
    return 0;
}
