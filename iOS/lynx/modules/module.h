// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_MODULES_MODULE_H_
#define UI_MODULES_MODULE_H_

#import <Foundation/Foundation.h>

#include "base/lynx_function_object.h"

@interface LxModule : LxFunctionObject

#define LX_REGISTER_MODULE(module) \
- (NSString *) moduleName { return @#module; }

- (NSString *) moduleName;

@end

#endif  // UI_MODULES_MODULE_H_
