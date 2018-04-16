// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/oc_property.h"

#import <objc/runtime.h>

#include "base/defines.h"
#include "base/oc_property_transformer.h"

#pragma mark -- LxOcPropertyInfo
@implementation LxOcPropertyInfo
    
@end

@implementation LxOcProperty {
    NSString *_signature;
    NSArray<LxOcPropertyInfo *> *_infos;
}

LX_NOT_IMPLEMENTED(- (instancetype) init)

- (instancetype)initWithInfos:(NSArray<LxOcPropertyInfo *> *)infos {
    self = [super init];
    if (self) {
        _signature = [LxOcPropertyTransformer transformTypes:infos];
    }
    return self;
}

- (instancetype)initWithInfo:(LxOcPropertyInfo *)info {
    self = [super init];
    if (self) {
        _signature = [LxOcPropertyTransformer transformType:info];
    }
    return self;
}

- (NSInteger)length {
    return [_infos count];
}

- (NSString *)transformedTypes {
    return _signature;
}

- (BOOL)isMatchedWith:(NSArray *)args {
    NSString *argsType = [LxOcPropertyTransformer transformTypeForObjects:args];
    if (argsType.length == _signature.length) {
        for (int i = 0; i < argsType.length; ++i) {
            char type = [argsType characterAtIndex:i];
            char wantedType = [_signature characterAtIndex:i];
            if (type == wantedType) continue;
            switch (wantedType) {
                case LxOcDoubleType:
                case LxOcIntType:
                case LxOcBoolType:
                    if (type != LxOcNSNumberType)
                        return NO;
                    break;
                case LxOcNSNumberType:
                    if (!(type == LxOcBoolType
                        || type == LxOcIntType
                        || type == LxOcDoubleType))
                        return NO;
                    break;
                case LxOcStringType:
                    if (type != LxOcNSStringType)
                        return NO;
                    break;
                case LxOcNSArrayType:
                case LxOcNSStringType:
                default:
                    break;
            }
        }
        return YES;
    }
    return NO;
}

- (int) convertProperty:(NSArray*) array toSuitable:(void*[]) args {
    int index = 0;
    for (id object : array) {
        char type = [_signature characterAtIndex:index];
        if ([object isKindOfClass:[NSNumber class]]) {
            switch (type) {
                case LxOcDoubleType: {
                    double v = ((NSNumber *)object).doubleValue;
                    args[index] = (void *) (double*) &v;
                }
                    break;
                case LxOcIntType: {
                    int v = ((NSNumber *)object).intValue;
                    args[index] = (void *) (int*) &v;
                }
                    break;
                case LxOcBoolType: {
                    BOOL v = ((NSNumber *)object).boolValue;
                    args[index] = (void *) (BOOL*) &v;
                }
                    break;
                case LxOcNSNumberType:
                    args[index] = (__bridge void *)object;
                    break;
                default:
                    break;
            }
        } else if ([object isKindOfClass:[NSString class]]) {
            switch (type) {
                case LxOcStringType:
                    args[index] = (void *)[((NSString *)object) UTF8String];
                    break;
                case LxOcNSStringType:
                    args[index] = (__bridge void *)object;
                    break;
                default:
                    break;
            }
        } else if ([object isKindOfClass:[NSArray class]]
                        && type == LxOcNSArrayType) {
            args[index] = (__bridge void *)object;
        } else if ([object isKindOfClass:[NSDictionary class]]
                        && type == LxOcNSDictionaryType) {
            args[index] = (__bridge void *)object;
        }
        index++;
    }
    return array ? (int) [array count] : 0;
}

@end
