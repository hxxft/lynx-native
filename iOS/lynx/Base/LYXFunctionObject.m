// Copyright 2017 The Lynx Authors. All rights reserved.
#import "LYXFunctionObject.h"

#import <objc/runtime.h>
#import "LYXOcMethod.h"
#import "LYXOcCenter.h"

@implementation LYXFunctionObject {
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
    if (clazz && ![[LYXOcCenter shareInstance] hasRegister:clazz]) {
        unsigned int methodCount;
        while (clazz != [NSObject class] && clazz != [NSProxy class]) {
            Method *methods = class_copyMethodList(object_getClass(clazz), &methodCount);
            
            for (unsigned int i = 0; i < methodCount; i++) {
                Method method = methods[i];
                SEL selector = method_getName(method);
                if ([NSStringFromSelector(selector) hasPrefix:LYX_METHOD_PREFIX]) {
                    IMP imp = method_getImplementation(method);
                    auto methodInfo = ((const LYXMethodInfo *(*)(id, SEL))imp)(clazz, selector);
                    LYXOcMethod *lynxMethod = [[LYXOcMethod alloc] initWithInfo:methodInfo andClass:clazz];
                    [self registerJSMethod:lynxMethod];
                }
            }
            
            free(methods);
            clazz = class_getSuperclass(clazz);
        }
    } else {
        
    }
}

- (void) registerJSMethod:(LYXOcMethod *) method {
    [_methodNames addObject: method.name];
    [[LYXOcCenter shareInstance] registerMethod:method];
}

- (id)execMethod:(NSString *)name andArgs:(NSArray *)args {
    LYXOcMethod *method = [[LYXOcCenter shareInstance] findMethodWithReceiver:self andMethod:name andArgs:args];
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
