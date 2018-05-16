// Copyright 2017 The Lynx Authors. All rights reserved.

#include "modules/module_register.h"
#include "modules/module_bundle.h"
#include "modules/module.h"
#include "base/defines.h"

#include "modules/coordinator_register.h"
#include "modules/page_navigator.h"

@implementation LxModuleRegister

LX_NOT_IMPLEMENTED(- (instancetype)init)

-(instancetype)initWithRuntime:(LynxRuntime *)runtime {
    self = [super init];
    if (self) {
        LxModuleBundle *bundle = [[LxModuleBundle alloc] initWithRuntime:runtime];
        for (LxModule *module in bundle.modulePacket) {
            [runtime addJavaScriptInterface:module withName:[module moduleName]];
        }
    }
    return self;
}

+ (void) registeWithRuntime: (LynxRuntime *) runtime {
    NSMutableArray<LxModule *> *modulePacket = [[NSMutableArray alloc] init];
    [modulePacket addObject:[[LxPageNavigator alloc] init]];
    [modulePacket addObject:[[LxCrdRegister alloc] initWithRuntime:runtime]];
    
    for (LxModule *module in modulePacket) {
        [runtime addJavaScriptInterface:module withName:[module moduleName]];
    }
}

@end
