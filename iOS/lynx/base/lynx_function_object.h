// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_BASE_LYNX_FUNCTION_OBJECT_H_
#define UI_BASE_LYNX_FUNCTION_OBJECT_H_

#import <Foundation/Foundation.h>

#include "base/oc_method.h"
#include "base/defines.h"

@interface LxFunctionObject : NSObject

#define LX_METHOD_PREFIX @"__lynx_method__"

#define LX_GENERATE_IDENTIFIABLE_METHOD(js_name, method) \
+ (const LxMethodInfo *) LX_CONCAT(__lynx_method__, LX_CONCAT(js_name, LX_CONCAT(__LINE__, __COUNTER__))) { \
    static LxMethodInfo info = {#js_name, #method}; \
    return &info; \
}

#define LX_REGISTER_METHOD(returnValue, method) \
LX_GENERATE_IDENTIFIABLE_METHOD(, method) \
- (returnValue) method

#define LX_REGISTER_METHOD_WName(js_name, returnValue, method) \
LX_GENERATE_IDENTIFIABLE_METHOD(js_name, method) \
- (returnValue) method

- (id) execMethod:(NSString *)name andArgs:(NSArray *) args;

- (NSArray<NSString*> *) methodNames;

- (void) onDestroy;

@end

#endif  // UI_BASE_LYNX_FUNCTION_OBJECT_H_
