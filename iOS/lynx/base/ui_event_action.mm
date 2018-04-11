// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/ui_event_action.h"

@implementation LynxUIEventAction

- (id) initWithTarget:(LynxRenderObjectImpl *)target andEvent:(NSString *)event andValue:(NSArray *)array {
    self = [super initWithType:DO_EVENT_ACTION andTarget:target];
    if (self) {
        _event = event;
        _array = array;
    }
    return self;
}

- (void)doAction {
    [self.renderObjectImpl dispatchEvent:_event withParam:_array];
}

@end
