// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_BASE_UI_EVENT_ACTION_H_
#define UI_BASE_UI_EVENT_ACTION_H_

#import <Foundation/Foundation.h>

#include "base/ui_action.h"

@interface LynxUIEventAction : LynxUIAction

@property(nonatomic, readwrite) NSString *event;
@property(nonatomic, readwrite) NSArray *array;

- (id) initWithTarget:(LynxRenderObjectImpl *)target andEvent:(NSString *)event andValue:(NSArray *)array;
@end

#endif  // UI_BASE_UI_EVENT_ACTION_H_
