// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_BASE_OC_CENTER_H_
#define UI_BASE_OC_CENTER_H_

#import <Foundation/Foundation.h>
#import <objc/runtime.h>

#include "base/oc_method.h"
#include "base/lynx_function_object.h"

@interface LYXOcCenter : NSObject

typedef  NSMutableArray<LYXOcMethod*> MethodArray;
typedef  NSMutableDictionary<NSString*, NSMutableArray<LYXOcMethod*>*> ClassMethodMap;
@property(nonatomic, readonly) NSMutableDictionary<NSString*, ClassMethodMap*> *methodMap;

@property(nonatomic, readonly) NSMutableDictionary<NSString*, LYXOcMethod*> *signatureMethodMap;

+ (instancetype) shareInstance;
- (BOOL) hasRegister:(Class) clazz;
- (void) registerMethod:(LYXOcMethod *) method;
- (LYXOcMethod *) findMethodWithReceiver:(id) object
                               andMethod:(NSString *) methodName
                                 andArgs:(NSArray *) array;

@end

#endif  // UI_BASE_OC_CENTER_H_
