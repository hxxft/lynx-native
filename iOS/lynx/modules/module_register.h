// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_MODULES_MODULE_REGISTER_H_
#define UI_MODULES_MODULE_REGISTER_H_

#import <Foundation/Foundation.h>

#include "modules/module_bundle.h"

@interface LxModuleRegister : NSObject

- (instancetype) initWithRuntime:(LynxRuntime *) runtime NS_DESIGNATED_INITIALIZER;

+ (void) registeWithRuntime: (LynxRuntime *) runtime;
@end



#endif  // UI_MODULES_MODULE_REGISTER_H_
