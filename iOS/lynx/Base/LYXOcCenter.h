// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import <objc/runtime.h>
#import "LYXOcMethod.h"
#import "LYXFunctionObject.h"

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
