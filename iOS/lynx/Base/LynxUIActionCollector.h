// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import "LynxUIAction.h"

@interface LynxUIActionCollector : NSObject

@property(nonatomic, readwrite) NSMutableDictionary *eventActions;
@property(nonatomic, readwrite) NSMutableDictionary *updateDataActions;

- (void) collectAction:(LynxUIAction *)action;
- (BOOL) needDoAction;
- (void) doActions;

@end
