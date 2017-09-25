// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "LynxRenderTreeHostImpl.h"

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
