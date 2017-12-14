// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LYXOcCenter.h"

@implementation LYXOcCenter

static LYXOcCenter *_singleLeton;

+ (instancetype) shareInstance {
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _singleLeton = [[self alloc] init];
    });
    
    return _singleLeton;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        _methodMap = [[NSMutableDictionary alloc] init];
        _signatureMethodMap = [[NSMutableDictionary alloc] init];
    }
    return self;
}

- (BOOL)hasRegister:(Class)clazz {
    return [[_methodMap allKeys] containsObject:clazz];
}

- (void) registerAsSignatureMethod:(LYXOcMethod *) method {
    [_signatureMethodMap setObject:method forKey:method.signature];
}

- (void)registerMethod:(LYXOcMethod *)method {
    ClassMethodMap * classMethodMap = _methodMap[method.clazz];
    if (!classMethodMap) {
        classMethodMap = [[NSMutableDictionary alloc] init];
        [_methodMap setObject:classMethodMap forKey:method.clazz];
    }
    MethodArray * methodArray = classMethodMap[method.name];
    if (!methodArray) {
        methodArray = [[NSMutableArray alloc] init];
        [classMethodMap setObject:methodArray forKey:method.name];
    }
    [methodArray addObject:method];
    
    [self registerAsSignatureMethod: method];
}

- (LYXOcMethod *)findMethodWithReceiver:(id)object
                     andMethod:(NSString *)methodName
                       andArgs:(NSArray *)array {
    LYXOcMethod *method = nil;
    NSString* signature = [LYXOcMethod generateSingatureWithReciever:[object class] methodName:methodName args:array];
    method = _signatureMethodMap[signature];
    if (method) {
        return method;
    }
    method = [self findMatchedMethodWithReceiver:object
                                       andMethod:methodName
                                         andArgs:array];
    return method;
}

- (LYXOcMethod *)findMatchedMethodWithReceiver:(id)object
                                     andMethod:(NSString *)methodName
                                       andArgs:(NSArray *)args {
    MethodArray *methodArray = _methodMap[[object class]][methodName];
    for (LYXOcMethod *method in methodArray) {
        if ([[method argumentTypes] isMatchedWith:args]) {
            return method;
        }
    }
    return nil;
}

@end
