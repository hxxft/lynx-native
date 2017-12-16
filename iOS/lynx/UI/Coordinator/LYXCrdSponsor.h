// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "LYXCrdObject.h"
#import "LYXCrdTypes.h"

@protocol LYXCrdSponsor <LYXCrdObject>
@required
@property(nonatomic, readonly) LYXCrdTypes *coordinatorTypes;

- (BOOL) dispatchCoordinatorScrollTop:(NSInteger) scrollTop
                              andLeft:(NSInteger) scrollLeft;

- (BOOL) dispatchCoordinatorTouchEvent:(UIEvent *) event
                                  type:(NSString *) type;

@end
