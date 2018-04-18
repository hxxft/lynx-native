// Copyright 2017 The Lynx Authors. All rights reserved.

#include "widget/swiper/lynx_swiper_controller.h"
#include "base/render_object_impl_bridge.h"

@implementation LxSwiperController

- (id) init:(LxSwiperView *) swiperView withUI:(LynxUI *)ui {
    self = [super init];
    if (self) {
        _swiperView = swiperView;
        _ui = ui;
        _children = [[NSMutableArray alloc] init];
    }
    return self;
}

- (void)reloadData {
    [_children removeAllObjects];
    RenderObjectImplBridge *impl = _ui.renderObjectImpl;
    for (RenderObjectImplBridge *child in impl.children) {
        [_children addObject:child];
    }
    [_swiperView reloadData];
}

- (NSInteger)numberOfViewsInSwiperView:(LxSwiperView *)swiperView {
    return _children.count;
}

- (UIView *)swiperView:(LxSwiperView *)swiperView viewAtIndex:(NSInteger)index {
    return [self buildChildView:[_children objectAtIndex:index]];
}

- (CGSize)tableView:(LxSwiperView *)swiperView sizeAtIndex:(NSInteger)index {
    RenderObjectImplBridge *child = _children[index];
    CGSize size;
    size.width = child->size_.width_;
    size.height = child->size_.height_;
    return size;
}

- (UIView *) buildChildView:(RenderObjectImplBridge *)impl {
    if (!impl.ui) {
        [impl createLynxUI];
    }
    UIView * view = impl.ui.view;
    return view;
}
 
@end
