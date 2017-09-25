// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LynxUIBody.h"
#import "LynxView.h"

@implementation LynxUIBody

- (UIView *)createView:(LynxRenderObjectImpl *)impl {
    return nil;
}

- (void)bindRenderObjectImpl:(LynxRenderObjectImpl *)impl {
    self.renderObjectImpl = impl;
}

- (void) resetView:(IOSView*) view {
    self.view = view;
    // Actually bind data
    [super bindRenderObjectImpl: self.renderObjectImpl];
}

- (void) collectAction:(LynxUIAction *)action {
    [((LynxView *)self.view).renderTreeHostImpl collectAction:action];
}

@end