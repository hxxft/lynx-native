// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LYXModuleRegister.h"
#import "LYXModuleBundle.h"
#import "LYXModule.h"
#import "LYXDefines.h"

@implementation LYXModuleRegister

LYX_NOT_IMPLEMENTED(- (instancetype)init)

-(instancetype)initWithRuntime:(LynxRuntime *)runtime {
    self = [super init];
    if (self) {
        LYXModuleBundle *bundle = [[LYXModuleBundle alloc] initWithRuntime:runtime];
        for (LYXModule *module in bundle.modulePacket) {
            [runtime addJavaScriptInterface:module withName:[module moduleName]];
        }
    }
    return self;
}

@end
