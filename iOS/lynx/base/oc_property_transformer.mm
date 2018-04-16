// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/oc_property_transformer.h"
#import <objc/runtime.h>

@implementation LxOcPropertyTransformer

+ (NSString *)transformTypes:(NSArray<LxOcPropertyInfo*> *)infos {
    NSMutableString* types = [[NSMutableString alloc] init];
    for (LxOcPropertyInfo *info in infos) {
        [types appendString:[self transformType:info]];
    }
    return types;
}

+ (NSString *)transformType:(LxOcPropertyInfo*)info {
    char type;
    switch(info.objCType[0]) {
        case _C_SHT:
        case _C_USHT:
        case _C_INT:
        case _C_UINT:
        case _C_LNG:
        case _C_ULNG:
        case _C_LNG_LNG:
        case _C_ULNG_LNG:
            type = LxOcIntType;
            break;
        case _C_FLT:
        case _C_DBL:
            type = LxOcDoubleType;
            break;
        case _C_BOOL:
            type = LxOcBoolType;
            break;
        case _C_CHARPTR:
            type = LxOcStringType;
            break;
        case _C_CHR:
        case _C_UCHR:
            type = LxOcCharType;
            break;
        case _C_ID: {
            if ([info.typeStr isEqualToString:@"NSString"]) {
                type = LxOcNSStringType;
            } else if ([info.typeStr isEqualToString:@"NSArray"]) {
                type = LxOcNSArrayType;
            } else if ([info.typeStr isEqualToString:@"NSDictionary"]) {
                type = LxOcNSDictionaryType;
            } else if ([info.typeStr isEqualToString:@"NSNumber"]) {
                type = LxOcNSNumberType;
            } else {
                throw [NSException exceptionWithName:@"LxOcProperty"
                                              reason:[NSString stringWithFormat:@"Property '%@'not support", info.typeStr]
                                            userInfo:nil];
            }
            break;
        }
        default:
            throw [NSException exceptionWithName:@"LxOcProperty"
                                          reason:[NSString stringWithFormat:@"Property '%@'not support", info.typeStr]
                                        userInfo:nil];
            break;
    }
    return [NSString stringWithFormat:@"%c", type];
    
}

+ (NSString *)transformTypeForObject:(id)object {
    char type;
    if ([object isKindOfClass:NSNumber.class]) {
        type = LxOcNSNumberType;
    } else if ([object isKindOfClass:NSArray.class]) {
        type = LxOcNSArrayType;
    } else if ([object isKindOfClass:NSDictionary.class]) {
        type = LxOcNSDictionaryType;
    } else if ([object isKindOfClass:NSString.class]) {
        type = LxOcNSStringType;
    } else {
        throw [NSException exceptionWithName:@"LxOcPropertyTransformer"
                                      reason:[NSString stringWithFormat:@"Property '%@'not support", NSStringFromClass([object class])]
                                    userInfo:nil];
    }
    return [NSString stringWithFormat:@"%c", type];
}

+ (NSString *)transformTypeForObjects:(NSArray *)objects {
    NSMutableString *types = [[NSMutableString alloc] init];
    for (id object in objects) {
        [types appendString:[self transformTypeForObject:object]];
    }
    return types;
}

@end
