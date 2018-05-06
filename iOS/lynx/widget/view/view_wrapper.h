// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_WIDGET_VIEW_VIEW_WRAPPER_H_
#define UI_WIDGET_VIEW_VIEW_WRAPPER_H_

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#include "widget/lynx_ui.h"

@interface ViewWrapper : UIView

@property(nonatomic, readwrite, weak) LynxUI *ui;
@property(nonatomic, readwrite) bool clickable;
@property(nonatomic) UITapGestureRecognizer* singleTap;

- (void)layoutSubviews;

- (id)initWithUI:(LynxUI *)ui;

- (void)addEvent:(NSString *)event;

- (void)removeEvent:(NSString *)event;

@end

@interface GestureHandler : UIGestureRecognizer

@property(nonatomic, readwrite, weak) LynxUI *ui;

- (id)initWithUI:(LynxUI *)ui;

@end

#endif  // UI_WIDGET_VIEW_VIEW_WRAPPER_H_
