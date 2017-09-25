// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LynxUIAction.h"

@implementation LynxUIAction

- (id) initWithType:(LynxUIActionType)type andTarget:(LynxRenderObjectImpl *)target {
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
