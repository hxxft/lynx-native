// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef timeout_callback_h
#define timeout_callback_h

#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>
#import <UIKit/UIKit.h>
#include "jscore_runtime.h"

@interface TimeoutCallback : NSObject
{
    @private
    base::WeakPtr<jscore::JSCoreRuntime> runtime_;
}

-(id) initWithContext: (JSContext *)context WithRuntime: (base::WeakPtr<jscore::JSCoreRuntime>) runtime;

@end

#endif /* timeout_callback_h */
