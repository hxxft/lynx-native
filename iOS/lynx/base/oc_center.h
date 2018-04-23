// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_BASE_OC_CENTER_H_
#define UI_BASE_OC_CENTER_H_

#import <Foundation/Foundation.h>
#import <objc/runtime.h>

#include "base/oc_method.h"
#include "base/lynx_function_object.h"

@interface LxOcCenter : NSObject

typedef  NSMutableArray<LxOcMethod*> MethodArray;
typedef  NSMutableDictionary<NSString*, MethodArray*> ClassMethodMap;
@property(nonatomic, readonly) NSMutableDictionary<NSString*, ClassMethodMap*> *methodMap;

@property(nonatomic, readonly) NSMutableDictionary<NSString*, LxOcMethod*> *signatureMethodMap;

+ (instancetype) shareInstance;
- (BOOL) hasRegister:(Class) clazz;
- (void) registerMethod:(LxOcMethod *) method;
- (LxOcMethod *) findMethodWithReceiver:(id) object
                               andMethod:(NSString *) methodName
                                 andArgs:(NSArray *) array;
- (ClassMethodMap*) findMethodsWithClazz: (Class) clazz;

@end

#endif  // UI_BASE_OC_CENTER_H_
