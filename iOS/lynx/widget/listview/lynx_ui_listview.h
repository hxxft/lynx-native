// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_WIDGET_LISTVIEW_LYNX_UI_LISTVIEW_H_
#define UI_WIDGET_LISTVIEW_LYNX_UI_LISTVIEW_H_

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include "widget/listview/listview_controller.h"
#include "widget/lynx_ui.h"

@interface LxUIListView : LynxUI<UITableView *>

@property(nonatomic, readonly) LynxListViewController *controller;

- (void) reloadData;
@end

#endif  // UI_WIDGET_LISTVIEW_LYNX_UI_LISTVIEW_H_
