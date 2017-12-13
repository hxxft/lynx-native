// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LYXModuleBundle.h"
#import "LYXPageNavigator.h"
#import "LYXCrdRegister.h"
#import "LYXModule.h"
#import "LYXDefines.h"

@implementation LYXModuleBundle

LYX_NOT_IMPLEMENTED(- (instancetype) init)

- (instancetype)initWithRuntime:(LynxRuntime *)runtime {
    self = [super init];
    if (self) {
        _modulePacket = [[NSMutableArray alloc] init];
        [_modulePacket addObject:[[LYXPageNavigator alloc] init]];
        [_modulePacket addObject:[[LYXCrdRegister alloc] initWithRuntime:runtime]];
    }
    return self;
}

@end
