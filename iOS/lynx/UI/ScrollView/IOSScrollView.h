// Copyright 2017 The Lynx Authors. All rights reserved.

#import <UIKit/UIKit.h>
#import "LynxUI.h"

@interface IOSScrollView : UIScrollView<UIScrollViewDelegate>

@property(nonatomic, readwrite, weak) LynxUI *ui;

- (void)layoutSubviews;
- (id)initWithUI:(LynxUI *)ui;
@end