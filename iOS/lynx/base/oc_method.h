// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_BASE_OC_METHOD_H_
#define UI_BASE_OC_METHOD_H_

#import <Foundation/Foundation.h>

#include "base/oc_property.h"

typedef struct LxMethodInfo {
    const char *const js_name;
    const char *const method;
} LxMethodInfo;

@interface LxOcMethod : NSObject

@property(nonatomic, readonly) Class clazz;
@property(nonatomic, readonly) const LxMethodInfo * info;
@property(nonatomic, readonly) NSString *signature;
@property(nonatomic, readonly) NSString *name;
@property(nonatomic, readonly) LxOcProperty *returnType;
@property(nonatomic, readonly) LxOcProperty *argumentTypes;
@property(nonatomic, readonly) SEL selector;

- (instancetype) initWithInfo:(const LxMethodInfo *)info
                     andClass:(Class) clazz NS_DESIGNATED_INITIALIZER;

- (id) invokeWithReceiver:(id) object
                  andArgs:(NSArray *) args;

+ (NSString *) generateSingatureWithReciever:(Class) clazz
                                  methodName:(NSString *)name
                                        args:(NSArray*) args;

@end

#endif
