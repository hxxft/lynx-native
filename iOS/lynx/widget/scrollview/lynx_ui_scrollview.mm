// Copyright 2017 The Lynx Authors. All rights reserved.

#include "widget/scrollview/lynx_ui_scrollview.h"
#include "base/lynx_render_object_impl.h"

#include "base/ios/common.h"

@implementation LxUIScrollView

static NSString * const kAttrPageEnable = @"page-enable";

- (id)createView:(LynxRenderObjectImpl *) impl {
    return [[ScrollViewWrapper alloc] initWithUI:self];
}

- (void)setAttribute:(NSString *)value forKey:(NSString *)key {
    [super setAttribute:value forKey:key];
    if ([key isEqualToString:kAttrPageEnable]) {
        ((ScrollViewWrapper *)self.view).pagingEnabled = [value boolValue];
    }
}

- (void) setSize:(const base::Size &)size {
    if(!self.view) return;
    self.view.contentSize = CGSizeMake(size.width_, size.height_);
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
    switch (attr) {
        case SCROLL_LEFT:
            [self.view setContentOffset:CGPointMake([SAFE_CONVERT(value, NSNumber) intValue], 0) animated:YES];
            break;
        case SCROLL_TOP:
            [self.view setContentOffset:CGPointMake(0, [SAFE_CONVERT(value, NSNumber) intValue]) animated:YES];
            break;
        default:
            break;
    }
}

@end
