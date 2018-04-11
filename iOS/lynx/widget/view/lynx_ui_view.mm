// Copyright 2017 The Lynx Authors. All rights reserved.

#include "widget/view/lynx_ui_view.h"

#include "base/lynx_render_object_impl.h"

#import <CoreGraphics/CGGeometry.h>

@implementation LxUIView

- (id) createView:(LynxRenderObjectImpl *) impl {
    return [[ViewWrapper alloc]initWithUI:self];;
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
