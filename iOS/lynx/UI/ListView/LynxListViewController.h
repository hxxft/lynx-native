// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "LynxRenderObjectImpl.h"
#import "LynxUI.h"

@interface LynxListViewController : NSObject <UITableViewDataSource, UITableViewDelegate>

@property(nonatomic, readonly, weak) UITableView *tableView;
@property(nonatomic, readwrite, assign) BOOL scrollEventEnable;
@property(nonatomic, readwrite, weak) LynxUI *ui;
@property(nonatomic, readwrite) NSMutableArray *children;

- (instancetype) init:(UITableView *)tableView withUI:(LynxUI *)ui;
- (void) reloadData:(LynxRenderObjectImpl *)impl;
@end

