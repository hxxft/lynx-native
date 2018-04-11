// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/framerate_controller.h"


@implementation LynxFrameRateController

-(instancetype)initWithVSyncListener:(LynxRenderTreeHostImpl *)vsync_listener
{
    self = [super init];
    if (self) {
        _displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(onVSync)];
        _vsync_listener = vsync_listener;
    }
    return self;
}

-(void)start
{
    _displayLink.paused = NO;
    [_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
}

-(void)stop
{
    _displayLink.paused = YES;
    [_displayLink removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
}

-(void)onVSync
{
//    vsync_listener_->OnVSync();
    [_vsync_listener onVSync];
    _displayLink.paused = NO;
}

@end
