// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_WIDGET_LISTVIEW_LISTVIEW_CONTROLLER_H_
#define UI_WIDGET_LISTVIEW_LISTVIEW_CONTROLLER_H_

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include "base/lynx_render_object_impl.h"
#include "widget/lynx_ui.h"

@interface LynxListViewController : NSObject <UITableViewDataSource, UITableViewDelegate>

@property(nonatomic, readonly, weak) UITableView *tableView;
@property(nonatomic, readwrite, assign) BOOL scrollEventEnable;
@property(nonatomic, readwrite, weak) LynxUI *ui;
@property(nonatomic, readwrite) NSMutableArray *children;

- (instancetype) init:(UITableView *)tableView withUI:(LynxUI *)ui;
- (void) reloadData:(LynxRenderObjectImpl *)impl;
@end

#endif  // UI_WIDGET_LISTVIEW_LISTVIEW_CONTROLLER_H_

