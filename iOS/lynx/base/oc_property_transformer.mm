// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/oc_property_transformer.h"
#import <objc/runtime.h>

@implementation LYXOcPropertyTransformer

+ (NSString *)transformTypes:(NSArray<LYXOcPropertyInfo*> *)infos {
    NSMutableString* types = [[NSMutableString alloc] init];
    for (LYXOcPropertyInfo *info in infos) {
        [types appendString:[self transformType:info]];
    }
    return types;
}

+ (NSString *)transformType:(LYXOcPropertyInfo*)info {
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
            type = LYXOcIntType;
            break;
        case _C_FLT:
        case _C_DBL:
            type = LYXOcDoubleType;
            break;
        case _C_BOOL:
            type = LYXOcBoolType;
            break;
        case _C_CHARPTR:
            type = LYXOcStringType;
            break;
        case _C_CHR:
        case _C_UCHR:
            type = LYXOcCharType;
            break;
        case _C_ID: {
            if ([info.typeStr isEqualToString:@"NSString"]) {
                type = LYXOcNSStringType;
            } else if ([info.typeStr isEqualToString:@"NSArray"]) {
                type = LYXOcNSArrayType;
            } else if ([info.typeStr isEqualToString:@"NSDictionary"]) {
                type = LYXOcNSDictionaryType;
            } else if ([info.typeStr isEqualToString:@"NSNumber"]) {
                type = LYXOcNSNumberType;
            } else {
                throw [NSException exceptionWithName:@"LYXOcProperty"
                                              reason:[NSString stringWithFormat:@"Property '%@'not support", info.typeStr]
                                            userInfo:nil];
            }
            break;
        }
        default:
            throw [NSException exceptionWithName:@"LYXOcProperty"
                                          reason:[NSString stringWithFormat:@"Property '%@'not support", info.typeStr]
                                        userInfo:nil];
            break;
    }
    return [NSString stringWithFormat:@"%c", type];
    
}

+ (NSString *)transformTypeForObject:(id)object {
    char type;
    if ([object isKindOfClass:NSNumber.class]) {
        type = LYXOcNSNumberType;
    } else if ([object isKindOfClass:NSArray.class]) {
        type = LYXOcNSArrayType;
    } else if ([object isKindOfClass:NSDictionary.class]) {
        type = LYXOcNSDictionaryType;
    } else if ([object isKindOfClass:NSString.class]) {
        type = LYXOcNSStringType;
    } else {
        throw [NSException exceptionWithName:@"LYXOcPropertyTransformer"
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
