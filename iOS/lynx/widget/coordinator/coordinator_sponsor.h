// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_WIDGET_COORDINATOR_COORDINATOR_SPONSOR_H_
#define UI_WIDGET_COORDINATOR_COORDINATOR_SPONSOR_H_

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include "widget/coordinator/coordinator_object.h"
#include "widget/coordinator/coordinator_types.h"

@protocol LYXCrdSponsor <LYXCrdObject>
@required
@property(nonatomic, readonly) LYXCrdTypes *coordinatorTypes;

- (BOOL) dispatchCoordinatorScrollTop:(NSInteger) scrollTop
                              andLeft:(NSInteger) scrollLeft;

- (BOOL) dispatchCoordinatorTouchEvent:(UIEvent *) event
                                  type:(NSString *) type;

@end

#endif  // UI_WIDGET_COORDINATOR_COORDINATOR_SPONSOR_H_
