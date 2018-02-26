// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "LynxListViewController.h"
#import "LynxUI.h"

@interface LynxUIListView : LynxUI<UITableView *>

@property(nonatomic, readonly) LynxListViewController *controller;

- (void) reloadData;
@end