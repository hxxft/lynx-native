// Copyright 2017 The Lynx Authors. All rights reserved.

#import "timeout_callback.h"

@implementation TimeoutCallback

-(id) initWithContext: (JSContext *)context WithRuntime: (base::WeakPtr<jscore::JSCoreRuntime>) runtime;
{
    self = [super init];
    if (self) {
        runtime_ = runtime;
        context[@"setTimeout"] = ^(JSValue* callback, NSNumber* time) { return [self setFunction:callback Timeout:time]; };
        context[@"setInterval"] = ^(JSValue* callback, NSNumber* time) { return [self setFunction:callback Interval:time]; };
        context[@"clearTimeout"] = ^(NSNumber* tag) { return [self clearTimeout:tag]; };
        context[@"clearInterval"] = ^(NSNumber* tag) { return [self clearTimeout:tag]; };
    }
    return self;
}

-(id) setFunction:(JSValue *) callback Timeout:(NSNumber *) ms
{
    if (runtime_.IsValid()) {
        runtime_.Get()->thread_manager()->RunOnJSThreadDelay(new jscore::JSFunctionWrap(callback), [ms intValue]);
    }
    return nil;
}

-(id) setFunction:(JSValue *) callback Interval:(NSNumber *) ms
{
    if (runtime_.IsValid()) {
        runtime_.Get()->thread_manager()->RunOnJSThreadInterval(new jscore::JSFunctionWrap(callback), [ms intValue]);
    }
    return nil;
}

-(id) clearTimeout:(NSNumber *) tag
{
    return nil;
}


@end
