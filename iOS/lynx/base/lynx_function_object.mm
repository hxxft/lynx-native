// Copyright 2017 The Lynx Authors. All rights reserved.
#include "base/lynx_function_object.h"

#import <objc/runtime.h>

#include "base/oc_method.h"
#include "base/oc_center.h"

@implementation LxFunctionObject {
    NSMutableArray<NSString*> *_methodNames;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        _methodNames = [[NSMutableArray alloc] init];
        [self initWithReceiver:self.class];
    }
    return self;
}

- (void) initWithReceiver:(Class) clazz {
    if(!clazz) return;
    [self registerOcCenterWithClazz: clazz];
    [self registerJSMethod: clazz];
}

- (void) registerOcCenterWithClazz:(Class) clazz {
    if(!clazz) return;
    if (![[LxOcCenter shareInstance] hasRegister:clazz]) {
        unsigned int methodCount;
        while (clazz != [NSObject class] && clazz != [NSProxy class]) {
            Method *methods = class_copyMethodList(object_getClass(clazz), &methodCount);
            
            for (unsigned int i = 0; i < methodCount; i++) {
                Method method = methods[i];
                SEL selector = method_getName(method);
                if ([NSStringFromSelector(selector) hasPrefix:LX_METHOD_PREFIX]) {
                    IMP imp = method_getImplementation(method);
                    auto methodInfo = ((const LxMethodInfo *(*)(id, SEL))imp)(clazz, selector);
                    LxOcMethod *lynxMethod = [[LxOcMethod alloc] initWithInfo:methodInfo andClass:clazz];
                    [[LxOcCenter shareInstance] registerMethod:lynxMethod];
                }
            }
            
            free(methods);
            clazz = class_getSuperclass(clazz);
        }
    }
}

- (void) registerJSMethod:(Class) clazz {
    ClassMethodMap* methods = [[LxOcCenter shareInstance] findMethodsWithClazz:clazz];
    
    NSEnumerator* keys = [methods keyEnumerator] ;
    NSString* key = nil;
    while(key = [keys nextObject]) {
        [_methodNames addObject: key];
    }
}

- (id)execMethod:(NSString *)name andArgs:(NSArray *)args {
    LxOcMethod *method = [[LxOcCenter shareInstance] findMethodWithReceiver:self andMethod:name andArgs:args];
    if (!method) {
        return NULL;
    } else {
        return [method invokeWithReceiver:self andArgs:args];
    }
}

- (NSArray<NSString *> *)methodNames {
    return _methodNames;
}

- (void)onDestroy {
    
}

@end
