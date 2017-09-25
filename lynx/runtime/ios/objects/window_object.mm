// Copyright 2017 The Lynx Authors. All rights reserved.

#import "window_object.h"

@implementation WindowObject

-(id) initWithContext: (JSContext *) context
{
    self = [super init];
    if (self) {
        context[@"devicePixelRatio"] = @(self.devicePixelRatio);
        context[@"open"] = ^(NSString *url) { return [self open:url]; };
        context[@"addEventListener"] = ^(NSString *event, JSValue *func, Boolean useCapture) { return [self addEvent:event WithListener:func UseCapture:useCapture]; };
        context[@"removeEventListener"] = ^(NSString *event, JSValue *func) { return [self removeEvent:event WithListener:func]; };
        context[@"dispatchEvent"] = ^(id event) { return [self dispatchEvent:event]; };
    }
    return self;
}

-(id)open:(NSString *)url
{
    return nil;
}

-(id)addEvent: (NSString *)event WithListener:(JSValue *) func UseCapture:(Boolean) useCapture
{
    return nil;
}

-(id)removeEvent: (NSString *)event WithListener:(JSValue *) func
{
    return nil;
}

-(id)dispatchEvent: (id) event
{
    return nil;
}

-(CGFloat) devicePixelRatio
{
    return [UIScreen mainScreen].scale;
}

@end
