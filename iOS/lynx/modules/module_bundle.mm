// Copyright 2017 The Lynx Authors. All rights reserved.
#include "modules/module_bundle.h"

#include "base/defines.h"
#include "modules/coordinator_register.h"
#include "modules/module_register.h"
#include "modules/module.h"
#include "modules/page_navigator.h"


@implementation LxModuleBundle

LX_NOT_IMPLEMENTED(- (instancetype) init)

- (instancetype)initWithRuntime:(LynxRuntime *)runtime {
    self = [super init];
    if (self) {
        _modulePacket = [[NSMutableArray alloc] init];
        [_modulePacket addObject:[[LxPageNavigator alloc] init]];
        [_modulePacket addObject:[[LxCrdRegister alloc] initWithRuntime:runtime]];
    }
    return self;
}

@end
