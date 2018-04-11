// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_BASE_LYNX_FUNCTION_OBJECT_H_
#define UI_BASE_LYNX_FUNCTION_OBJECT_H_

#import <Foundation/Foundation.h>

#include "base/oc_method.h"
#include "base/defines.h"

@interface LYXFunctionObject : NSObject

#define LYX_METHOD_PREFIX @"__lynx_method__"

#define LYX_GENERATE_IDENTIFIABLE_METHOD(js_name, method) \
+ (const LYXMethodInfo *) LYX_CONCAT(__lynx_method__, LYX_CONCAT(js_name, LYX_CONCAT(__LINE__, __COUNTER__))) { \
    static LYXMethodInfo info = {#js_name, #method}; \
    return &info; \
}

#define LYX_REGISTER_METHOD(returnValue, method) \
LYX_GENERATE_IDENTIFIABLE_METHOD(, method) \
- (returnValue) method

#define LYX_REGISTER_METHOD_WName(js_name, returnValue, method) \
LYX_GENERATE_IDENTIFIABLE_METHOD(js_name, method) \
- (returnValue) method

- (id) execMethod:(NSString *)name andArgs:(NSArray *) args;

- (NSArray<NSString*> *) methodNames;

- (void) onDestroy;

@end

#endif  // UI_BASE_LYNX_FUNCTION_OBJECT_H_
