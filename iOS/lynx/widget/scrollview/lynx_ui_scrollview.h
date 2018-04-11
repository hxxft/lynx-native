// Copyright 2017 The Lynx Authors. All rights reserved.
#ifndef UI_WIDGET_SCROLLVIEW_LYNX_UI_SCROLLVIEW_H_
#define UI_WIDGET_SCROLLVIEW_LYNX_UI_SCROLLVIEW_H_

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include "widget/lynx_ui.h"
#include "widget/scrollview/scrollview_wrapper.h"

@interface LxUIScrollView : LynxUI<ScrollViewWrapper *>

@end

#endif  // UI_WIDGET_SCROLLVIEW_LYNX_UI_SCROLLVIEW_H_
