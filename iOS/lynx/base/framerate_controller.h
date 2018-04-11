// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_BASE_FRAMERATE_CONTROLLER_H_
#define UI_BASE_FRAMERATE_CONTROLLER_H_

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#include "base/lynx_render_tree_host_impl.h"

#include "render_tree_host_impl_ios.h"

@interface LynxFrameRateController : NSObject
{
@private
    lynx::RenderTreeHostImpl::VSyncListener* vsync_listener_;
}
@property(nonatomic, readwrite, weak) LynxRenderTreeHostImpl *vsync_listener;
@property(nonatomic, readwrite) CADisplayLink* displayLink;
-(instancetype)initWithVSyncListener:(LynxRenderTreeHostImpl *) vsync_listener;
-(void) start;
-(void) stop;
@end

#endif  // UI_BASE_FRAMERATE_CONTROLLER_H_
