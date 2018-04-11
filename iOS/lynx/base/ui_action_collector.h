// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_BASE_UI_ACTION_COLLECTOR_H_
#define UI_BASE_UI_ACTION_COLLECTOR_H_

#import <Foundation/Foundation.h>
#include "base/ui_action.h"

@interface LynxUIActionCollector : NSObject

@property(nonatomic, readwrite) NSMutableDictionary *eventActions;
@property(nonatomic, readwrite) NSMutableDictionary *updateDataActions;

- (void) collectAction:(LynxUIAction *)action;
- (BOOL) needDoAction;
- (void) doActions;

@end

#endif  // UI_BASE_UI_ACTION_COLLECTOR_H_
