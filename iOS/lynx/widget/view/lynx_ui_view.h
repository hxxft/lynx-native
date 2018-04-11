// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_WIDGET_VIEW_LYNX_UI_VIEW_H_
#define UI_WIDGET_VIEW_LYNX_UI_VIEW_H_

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include "widget/lynx_ui.h"
#include "widget/view/view_wrapper.h"

@interface LxUIView : LynxUI<ViewWrapper *>

@end

#endif  // UI_WIDGET_VIEW_LYNX_UI_VIEW_H_
