// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LynxUIScrollView.h"
#import "IOSScrollView.h"
#import "LynxRenderObjectImpl.h"

#include "base/ios/common.h"

@implementation LynxUIScrollView

- (UIView *)createView:(LynxRenderObjectImpl *) impl {
    return [[IOSScrollView alloc] initWithUI:self];
}

- (void) setSize:(const base::Size &)size {
    if(!self.view) return;
    ((IOSScrollView *)self.view).contentSize = CGSizeMake(size.width_, size.height_);
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

- (void) setData:(id) value withKey:(LynxRenderObjectAttr) attr {
    IOSScrollView *scrollView = SAFE_CONVERT(self.view, IOSScrollView);
    switch (attr) {
        case SCROLL_LEFT:
            [scrollView setContentOffset:CGPointMake([SAFE_CONVERT(value, NSNumber) intValue], 0) animated:YES];
            break;
        case SCROLL_TOP:
            [scrollView setContentOffset:CGPointMake(0, [SAFE_CONVERT(value, NSNumber) intValue]) animated:YES];
            break;
        default:
            break;
    }
}

@end
