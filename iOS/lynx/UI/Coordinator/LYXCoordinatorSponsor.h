// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "LYXCoordinatorObject.h"
#import "LYXCoordinatorTypes.h"

@protocol LYXCoordinatorSponsor <LYXCoordinatorObject>
@required
@property(nonatomic, readonly) LYXCoordinatorTypes *coordinatorTypes;

- (BOOL) dispatchCoordinatorScrollTop:(NSInteger) scrollTop
                              andLeft:(NSInteger) scrollLeft;

- (BOOL) dispatchCoordinatorTouchEvent:(UIEvent *) event
                                  type:(NSString *) type;

@end
