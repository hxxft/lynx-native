// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_WIDGET_COORDINATOR_COORDINATOR_ACTION_EXECUTOR_H_
#define UI_WIDGET_COORDINATOR_COORDINATOR_ACTION_EXECUTOR_H_

#import <Foundation/Foundation.h>
#include "widget/lynx_ui.h"

#include "render/coordinator/coordinator_action.h"

@interface LxCrdActionExecutor : NSObject

@property(nonatomic, readwrite, weak) LynxUI *ui;

- (instancetype) initWithUI:(LynxUI *) ui;

- (void) executeAction:(lynx::CoordinatorAction) action;
@end

#endif  // UI_WIDGET_COORDINATOR_COORDINATOR_ACTION_EXECUTOR_H_
