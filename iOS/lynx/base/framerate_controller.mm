// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/framerate_controller.h"

@interface LynxWeakProxy : NSProxy

@property (weak, nonatomic) id  target;

+ (LynxWeakProxy*)weakProxyWithTarget:(id)target;

@end

@implementation LynxWeakProxy

+ (LynxWeakProxy*)weakProxyWithTarget:(id)target
{
    LynxWeakProxy* proxy = [LynxWeakProxy alloc];
    proxy.target = target;
    return proxy;
}

- (BOOL)respondsToSelector:(SEL)sel
{
    return [_target respondsToSelector:sel];
}

- (id)forwardingTargetForSelector:(SEL)sel
{
    return _target;
}

- (void)forwardInvocation:(NSInvocation *)invocation {
    [invocation invokeWithTarget:self.target];
}

- (NSMethodSignature *)methodSignatureForSelector:(SEL)aSelector {
    NSMethodSignature *sig;
    sig = [self.target methodSignatureForSelector:aSelector];
    return sig;
}

@end



@interface LynxFrameRateController ()

@property (nonatomic, strong) LynxWeakProxy * proxyTarget;

@end

@implementation LynxFrameRateController

-(instancetype)initWithVSyncListener:(RenderTreeHostImplBridge *)vsync_listener
{
    self = [super init];
    if (self) {
        _proxyTarget = [LynxWeakProxy weakProxyWithTarget:self];
        _displayLink = [CADisplayLink displayLinkWithTarget:_proxyTarget selector:@selector(onVSync)];
        _vsync_listener = vsync_listener;
        [_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
    }
    return self;
}

-(void)start
{
    _displayLink.paused = NO;
}

-(void)stop
{
    _displayLink.paused = YES;
}

-(void)onVSync
{
//    vsync_listener_->OnVSync();
    [_vsync_listener onVSync];
    _displayLink.paused = NO;
}

-(void)dealloc
{
    [_displayLink invalidate];
    _displayLink = nil;
}

@end
