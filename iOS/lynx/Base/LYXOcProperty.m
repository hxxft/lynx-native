// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LYXOcProperty.h"
#import <objc/runtime.h>
#import "LYXDefines.h"
#import "LYXOcPropertyTransformer.h"

#pragma mark -- LYXOcPropertyInfo
@implementation LYXOcPropertyInfo
    
@end

@implementation LYXOcProperty {
    NSString *_signature;
    NSArray<LYXOcPropertyInfo *> *_infos;
}

LYX_NOT_IMPLEMENTED(- (instancetype) init)

- (instancetype)initWithInfos:(NSArray<LYXOcPropertyInfo *> *)infos {
    self = [super init];
    if (self) {
        _signature = [LYXOcPropertyTransformer transformTypes:infos];
    }
    return self;
}

- (instancetype)initWithInfo:(LYXOcPropertyInfo *)info {
    self = [super init];
    if (self) {
        _signature = [LYXOcPropertyTransformer transformType:info];
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
    NSString *argsType = [LYXOcPropertyTransformer transformTypeForObjects:args];
    if (argsType.length == _signature.length) {
        for (int i = 0; i < argsType.length; ++i) {
            char type = [argsType characterAtIndex:i];
            char wantedType = [_signature characterAtIndex:i];
            if (type == wantedType) continue;
            switch (wantedType) {
                case LYXOcDoubleType:
                case LYXOcIntType:
                case LYXOcBoolType:
                    if (type != LYXOcNSNumberType)
                        return NO;
                    break;
                case LYXOcNSNumberType:
                    if (!(type == LYXOcBoolType
                        || type == LYXOcIntType
                        || type == LYXOcDoubleType))
                        return NO;
                    break;
                case LYXOcStringType:
                    if (type != LYXOcNSStringType)
                        return NO;
                    break;
                case LYXOcNSArrayType:
                case LYXOcNSStringType:
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
                case LYXOcDoubleType: {
                    double v = ((NSNumber *)object).doubleValue;
                    args[index] = (void *) (double*) &v;
                }
                    break;
                case LYXOcIntType: {
                    int v = ((NSNumber *)object).intValue;
                    args[index] = (void *) (int*) &v;
                }
                    break;
                case LYXOcBoolType: {
                    BOOL v = ((NSNumber *)object).boolValue;
                    args[index] = (void *) (BOOL*) &v;
                }
                    break;
                case LYXOcNSNumberType:
                    args[index] = (__bridge void *)object;
                    break;
                default:
                    break;
            }
        } else if ([object isKindOfClass:[NSString class]]) {
            switch (type) {
                case LYXOcStringType:
                    args[index] = (void *)[((NSString *)object) UTF8String];
                    break;
                case LYXOcNSStringType:
                    args[index] = (__bridge void *)object;
                    break;
                default:
                    break;
            }
        } else if ([object isKindOfClass:[NSArray class]]
                        && type == LYXOcNSArrayType) {
            args[index] = (__bridge void *)object;
        } else if ([object isKindOfClass:[NSDictionary class]]
                        && type == LYXOcNSDictionaryType) {
            args[index] = (__bridge void *)object;
        }
        index++;
    }
    return array ? (int) [array count] : 0;
}

@end
