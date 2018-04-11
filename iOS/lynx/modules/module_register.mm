// Copyright 2017 The Lynx Authors. All rights reserved.

#include "modules/module_register.h"
#include "modules/module_bundle.h"
#include "modules/module.h"
#include "base/defines.h"

@implementation LYXModuleRegister

LYX_NOT_IMPLEMENTED(- (instancetype)init)

-(instancetype)initWithRuntime:(LynxRuntime *)runtime {
    self = [super init];
    if (self) {
        LYXModuleBundle *bundle = [[LYXModuleBundle alloc] initWithRuntime:runtime];
        for (LxModule *module in bundle.modulePacket) {
            [runtime addJavaScriptInterface:module withName:[module moduleName]];
        }
    }
    return self;
}

@end
