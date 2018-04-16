// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_MODULES_MODULE_BUNDLE_H_
#define UI_MODULES_MODULE_BUNDLE_H_

#import <Foundation/Foundation.h>

#include "base/lynx_runtime.h"

@class LxModule;

@interface LxModuleBundle : NSObject

@property(nonatomic, readonly) NSMutableArray<LxModule *> *modulePacket;

- (instancetype)initWithRuntime:(LynxRuntime *) runtime NS_DESIGNATED_INITIALIZER;

@end

#endif  // UI_MODULES_MODULE_BUNDLE_H_
