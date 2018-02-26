// Copyright 2017 The Lynx Authors. All rights reserved.

#import <CoreGraphics/CGGeometry.h>
#import "LynxUIView.h"
#import "LynxRenderObjectImpl.h"

@implementation LynxUIView

- (id) createView:(LynxRenderObjectImpl *) impl {
    return [[IOSView alloc]initWithUI:self];;
}

- (void) insertChild:(LynxRenderObjectImpl *)child atIndex:(int)index {
    if (!child.ui) {
        [child createLynxUI];
    }
    if(index != -1) {
        [self.view insertSubview:child.ui.view atIndex:index];
    }else {
        [self.view addSubview:child.ui.view];
    }
}

- (void) addEventListener:(NSString *)event {
    [self.view addEvent:event];
}

- (void) removeEventListener:(NSString *)event {
    [self.view removeEvent:event];
}

@end
