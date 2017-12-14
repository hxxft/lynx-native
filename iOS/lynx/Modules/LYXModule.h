// Copyright 2017 The Lynx Authors. All rights reserved.
#import <Foundation/Foundation.h>
#import "LYXFunctionObject.h"

@interface LYXModule : LYXFunctionObject

#define LYX_REGISTER_MODULE(module) \
- (NSString *) moduleName { return @#module; }

- (NSString *) moduleName;

@end
