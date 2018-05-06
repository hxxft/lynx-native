// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_BASE_FRAMERATE_CONTROLLER_H_
#define UI_BASE_FRAMERATE_CONTROLLER_H_

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#include "base/render_tree_host_impl_bridge.h"

#include "render_tree_host_impl_ios.h"

@interface LynxFrameRateController : NSObject
{
@private
    lynx::RenderTreeHostImpl::VSyncListener* vsync_listener_;
}
@property(nonatomic, readwrite, weak) RenderTreeHostImplBridge *vsync_listener;
@property(nonatomic, readwrite, weak) CADisplayLink* displayLink;
-(instancetype)initWithVSyncListener:(RenderTreeHostImplBridge *) vsync_listener;
-(void) start;
-(void) stop;
@end

#endif  // UI_BASE_FRAMERATE_CONTROLLER_H_
