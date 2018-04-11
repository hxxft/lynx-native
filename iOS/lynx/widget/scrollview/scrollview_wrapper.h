// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_WIDGET_SCROLLVIEW_SCROLLVIEW_WRAPPER_H_
#define UI_WIDGET_SCROLLVIEW_SCROLLVIEW_WRAPPER_H_

#import <UIKit/UIKit.h>
#include "widget/lynx_ui.h"

@interface ScrollViewWrapper : UIScrollView<UIScrollViewDelegate>

@property(nonatomic, readwrite, weak) LynxUI *ui;

- (void)layoutSubviews;
- (id)initWithUI:(LynxUI *)ui;
@end

#endif  // UI_WIDGET_SCROLLVIEW_SCROLLVIEW_WRAPPER_H_
