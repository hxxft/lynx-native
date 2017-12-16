// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LYXCrdCommandExecutor.h"

#include "render/coordinator/coordinator_executor.h"
#include "base/debug/memory_debug.h"

@interface LYXCrdCommandExecutor() {
    lynx::CoordinatorExecutor *_cExecutor;
}

@end

@implementation LYXCrdCommandExecutor

- (id)initWithExecutableContent:(NSString *)content {
    self = [super init];
    if (self) {
        _executable = content;
        _cExecutor = lynx_new lynx::CoordinatorExecutor([_executable UTF8String]);
    }
    return self;
}

- (void)updateExecutableContent:(NSString *)executable {
    if (![executable isEqualToString:_executable]) {
        _executable = executable;
        if (_cExecutor != NULL) {
            [self stopCExecutor];
        }
        _cExecutor = lynx_new lynx::CoordinatorExecutor([_executable UTF8String]);
    }
}

- (void)stopCExecutor {
    if (_cExecutor) {
        lynx_delete(_cExecutor);
    }
}

- (lynx::CoordinatorAction)executeCommandWithMethod:(NSString *)method
                                             andTag:(NSString *)tag
                                            andArgs:(double*)args
                                          andLength:(int) length {
    
    std::vector<lepus::Value> lepus_args;
    
    lepus::Value lepus_tag;
    lepus_tag.type_ = lepus::Value_String;
    if (tag) {
        lepus_tag.str_ = _cExecutor->context()
        ->string_pool()
        ->NewString([tag UTF8String]);
    } else {
        lepus_tag.str_ = lepus_tag.str_ = _cExecutor->context()
        ->string_pool()
        ->NewString("");
    }
    lepus_tag.str_->AddRef();
    
    lepus_args.push_back(lepus_tag);
    for (int i = 0; i < length; ++i) {
        lepus_args.push_back(args[i]);
    }
    return _cExecutor->Execute([method UTF8String], lepus_args);
}


- (BOOL) updateProperty:(NSString *) property withStr:(NSString *) value {
    if (property && property.length > 0) {
        std::string name = [property UTF8String];
        lepus::Value lepus_value;
        // Add string
        lepus_value.type_ = lepus::Value_String;
        if (value != NULL) {
            lepus_value.str_ = _cExecutor->context()->string_pool()
            ->NewString([value UTF8String]);
        } else {
            lepus_value.str_ = lepus_value.str_ = _cExecutor->context()->string_pool()->NewString("");
        }
        lepus_value.str_->AddRef();
        if (_cExecutor->context()->UpdateTopLevelVariable(name, lepus_value)) {
            return YES;
        }
    }
    return NO;
}

- (BOOL) updateProperty:(NSString *) property withBool:(BOOL) value {
    // Add boolean
    std::string name = [property UTF8String];
    lepus::Value lepus_value;
    lepus_value.type_ = lepus::Value_Boolean;
    lepus_value.boolean_ = value;
    if (_cExecutor->context()->UpdateTopLevelVariable(name, lepus_value)) {
        return YES;
    } else {
        return NO;
    }
}

- (BOOL) updateProperty:(NSString *) property withDouble:(double) value {
    
    // Add double
    std::string name = [property UTF8String];
    lepus::Value lepus_value = value;
    if (_cExecutor->context()->UpdateTopLevelVariable(name, lepus_value)) {
        return YES;
    } else {
        return NO;
    }
}


@end
