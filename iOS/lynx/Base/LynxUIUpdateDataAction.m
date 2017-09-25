// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LynxUIUpdateDataAction.h"

@implementation LynxUIUpdateDataAction

- (id) initWithTarget:(LynxRenderObjectImpl *)target andKey:(LynxRenderObjectAttr)key andData:(id)data {
    self = [super initWithType:DO_UPDATE_DATA_ACTION andTarget:target];
    if (self) {
        _key = key;
        _data = data;
    }
    return self;
}

- (void)doAction {
    [self.renderObjectImpl updateData:_data withKey:_key];
}

@end
