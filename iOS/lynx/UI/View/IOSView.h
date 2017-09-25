// Copyright 2017 The Lynx Authors. All rights reserved.

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "LynxUI.h"

@interface IOSView : UIView

@property(nonatomic, readwrite) LynxUI *ui;
@property UITapGestureRecognizer* singleTap;

- (void)layoutSubviews;

- (id)initWithUI:(LynxUI *)ui;

- (void)addEvent:(NSString *)event;

- (void)removeEvent:(NSString *)event;

@end
