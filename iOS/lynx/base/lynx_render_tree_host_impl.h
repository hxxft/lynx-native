// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_BASE_LYNX_RENDER_TREE_HOST_IMPL_H_
#define UI_BASE_LYNX_RENDER_TREE_HOST_IMPL_H_

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#include "base/lynx_render_object_impl.h"

#include "base/ui_action.h"
#include "base/ui_action_collector.h"

namespace lynx {
    class RenderTreeHostImplIOS;
}

@interface LynxRenderTreeHostImpl : NSObject {
    @private
    lynx::RenderTreeHostImplIOS* render_tree_host_impl_;
}

@property(nonatomic, readonly) LynxRenderObjectImpl *rootRenderObjectImpl;
@property(nonatomic, readwrite) LynxUIActionCollector *collector;
@property(nonatomic, readwrite) LynxUIActionCollector *lastCollector;

- (id) initWithRenderTreeHost:(lynx::RenderTreeHostImplIOS *) renderTreeHostImpl
          andRenderObjectImpl:(lynx::RenderObjectImplIOS *) renderObjectImpl;

- (void) onVSync;

- (void) beginFrame;

- (void) prepareCommit;

- (void) updateViewport:(CGRect)frame;

- (void) collectAction:(LynxUIAction *)action;
@end

#endif  // UI_BASE_LYNX_RENDER_TREE_HOST_IMPL_H_
