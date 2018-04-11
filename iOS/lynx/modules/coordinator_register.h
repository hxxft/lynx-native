// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_MODULES_COORDINATOR_REGISTER_H_
#define UI_MODULES_COORDINATOR_REGISTER_H_

#import <Foundation/Foundation.h>

#include "base/lynx_runtime.h"

#include "modules/module.h"

@interface LYXCrdRegister : LxModule

- (instancetype)initWithRuntime:(LynxRuntime *) runtime;

@end

#endif  // CONTENT_MODULES_COORDINATOR_REGISTER_H_
