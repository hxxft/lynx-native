// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/ui_action.h"

@implementation LynxUIAction

- (id) initWithType:(LynxUIActionType)type andTarget:(RenderObjectImplBridge *)target {
    self = [super init];
    if (self) {
        _renderObjectImpl = target;
        _type = type;
    }
    return self;
}

- (void)doAction {
    
}

@end
