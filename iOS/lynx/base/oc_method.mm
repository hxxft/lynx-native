// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/oc_method.h"
#include "base/defines.h"
#include "base/oc_property_transformer.h"

@implementation LYXOcMethod
{
    NSInvocation *_invocation;
}

LYX_NOT_IMPLEMENTED(-(instancetype) init)

- (void) skipWhiteSpace: (const char**) input {
    while (isspace(**input)) {
        (*input)++;
    }
}

- (NSString *) parseLegalStr: (const char**) input {
    const char* start = *input;
    if (!(isalpha(**input) || **input == '_'))
        return NULL;
    while (isalnum(**input) || **input == '_') {
        (*input)++;
    }
    return [self generateStrStart: start end:*input];
}

- (NSString *) generateStrStart:(const char*) start end:(const char*) end {
    return [[NSString alloc] initWithBytes:start
                                    length:(NSInteger) (end - start)
                                  encoding:(NSStringEncoding)NSASCIIStringEncoding];
}

- (void) parseMethodInfo {
    const char * method = _info->method;
    NSMutableString *selStr = [[NSMutableString alloc] init];
    NSMutableArray<NSString*> *argArray = [[NSMutableArray alloc] init];
    
    while (true) {
        [self skipWhiteSpace:&method];
        // Parse selector
        NSString *temp = [self parseLegalStr:&method];
        if (temp) {
            [selStr appendString:temp];
        } else {
            break;
        }
        
        [self skipWhiteSpace:&method];
        if (*(method++) == ':') {
            [selStr appendString:@":"];
        } else {
            break;
        }
        [self skipWhiteSpace:&method];
        
        // Parse argument type
        if (*(method++) == '(') {
            while (true) {
                [self skipWhiteSpace:&method];
                // skip nonnull / _Nonnull / nullable / _Nullable
                NSString* argType = [self parseLegalStr:&method];
                
                if (![temp isEqualToString:@"nonnull"]
                        && ![temp isEqualToString:@"_Nonnull"]
                        && ![temp isEqualToString:@"nullable"]
                        && ![temp isEqualToString:@"_Nullable"]) {
                    [argArray addObject:argType];
                    while (*(method++) != ')');
                    break;
                }
            }
        } else {
            break;
        }
        
        [self skipWhiteSpace:&method];
        // Parse argument name
        [self parseLegalStr:&method];
    }
    
    // Generate invocation
    _selector = NSSelectorFromString(selStr);
    NSMethodSignature *methodSignature = [_clazz instanceMethodSignatureForSelector:_selector];
    NSInvocation *invocation = [NSInvocation invocationWithMethodSignature:methodSignature];
    invocation.selector = _selector;
    _invocation = invocation;
    
    // Generate argument types
    NSMutableArray<LYXOcPropertyInfo*> *propertyInfos = [[NSMutableArray alloc] init];
    for (int i = 0; i < [methodSignature numberOfArguments] - 2; ++i) {
        LYXOcPropertyInfo *info = [[LYXOcPropertyInfo alloc] init];
        info.typeStr = argArray[i];
        info.objCType = [methodSignature getArgumentTypeAtIndex:i + 2];
        [propertyInfos addObject:info];
    }
    
    _argumentTypes = [[LYXOcProperty alloc] initWithInfos:propertyInfos];
    // Generate return type
    const char * temp = methodSignature.methodReturnType;
}

+ (NSString *)generateSingatureWithReciever:(Class)clazz
                                 methodName:(NSString *)name
                                       args:(NSArray*)args {
    return [NSString stringWithFormat:@"%@,%@,%@", NSStringFromClass(clazz), name, [LYXOcPropertyTransformer transformTypeForObjects:args]];;
}

- (NSString *) generateSingature {
    return [NSString stringWithFormat:@"%@,%@,%@", NSStringFromClass(_clazz), _name, [_argumentTypes transformedTypes]];;
}

- (NSString *) extractMethodName {
    NSString *name;
    if (_info->js_name && strlen(_info->js_name) > 0) {
        name = [NSString stringWithUTF8String:_info->js_name];
    } else {
        name = [NSString stringWithUTF8String:_info->method];
        NSRange colonRange = [name rangeOfString:@":"];
        if (colonRange.location != NSNotFound) {
            name = [name substringToIndex:colonRange.location];
        }
        name = [name stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
    }
    return name;
}

- (instancetype)initWithInfo:(const LYXMethodInfo *)info
                    andClass:(Class)clazz {
    self = [super init];
    if (self) {
        _info = info;
        _clazz = clazz;
        _name = [self extractMethodName];
        [self parseMethodInfo];
        _signature = [self generateSingature];
    }
    return self;
}

- (id)invokeWithReceiver:(id)object
                 andArgs:(NSArray *) args {
    void* convertedArgs[args.count];
    // Convert args
    [_argumentTypes convertProperty:args toSuitable:convertedArgs];
    for (int i = 0; i < args.count; ++i) {
        [_invocation setArgument:&convertedArgs[i] atIndex:i + 2];
    }
    // Invoke
    [_invocation invokeWithTarget:object];
    // Return value
//    void *returnValue = NULL;
//    [_invocation getReturnValue:&returnValue];
//        return nil;
//        return (__bridge id)returnValue;
    return nil;
}

@end
