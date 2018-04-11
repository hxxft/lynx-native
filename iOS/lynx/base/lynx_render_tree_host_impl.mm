// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/lynx_render_tree_host_impl.h"

#include "render/ios/render_tree_host_impl_ios.h"
#include "render/ios/render_object_impl_ios.h"

@implementation LynxRenderTreeHostImpl

- (id) initWithRenderTreeHost:(lynx::RenderTreeHostImplIOS *) renderTreeHostImpl
          andRenderObjectImpl:(lynx::RenderObjectImplIOS *) renderObjectImpl {
    self = [super init];
    if (self) {
        render_tree_host_impl_ = renderTreeHostImpl;
        _rootRenderObjectImpl = renderObjectImpl->ios();
        _collector = [[LynxUIActionCollector alloc] init];
    }
    return self;
}

- (void) onVSync {
    render_tree_host_impl_->OnVSync();
}

- (void) beginFrame {
    _lastCollector = _collector;
    _collector = [[LynxUIActionCollector alloc] init];
}

- (void) prepareCommit {
    if (_lastCollector && [_lastCollector needDoAction]) {
        [_lastCollector doActions];
    }
}

- (void) updateViewport:(CGRect) frame {
    int left = frame.origin.x;
    int top = frame.origin.y;
    int right = left + frame.size.width;
    int bottom = top + frame.size.height;
    render_tree_host_impl_->UpdateViewport(left, top, right, bottom);
}

- (void) collectAction:(LynxUIAction *)action {
    [_collector collectAction:action];
}

@end
