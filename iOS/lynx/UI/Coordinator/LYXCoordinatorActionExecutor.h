// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import "LynxUI.h"

#include "render/coordinator/coordinator_action.h"

@interface LYXCoordinatorActionExecutor : NSObject

- (instancetype) initWithUI:(LynxUI *) ui;

- (void) executeAction:(lynx::CoordinatorAction) action;
@end
