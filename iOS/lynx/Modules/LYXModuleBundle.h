// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>

#import "LynxRuntime.h"

@class LYXModule;

@interface LYXModuleBundle : NSObject

@property(nonatomic, readonly) NSMutableArray<LYXModule *> *modulePacket;

- (instancetype)initWithRuntime:(LynxRuntime *) runtime NS_DESIGNATED_INITIALIZER;

@end
