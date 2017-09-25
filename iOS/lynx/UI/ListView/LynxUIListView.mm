// Copyright 2017 The Lynx Authors. All rights reserved.

#include "LynxUIListView.h"
#include "base/ios/common.h"

@implementation LynxUIListView

- (UIView *)createView:(LynxRenderObjectImpl *)impl {
    UITableView *view = [[UITableView alloc] init];
    _controller = [[LynxListViewController alloc] init:(UITableView *)view withUI:self];
    view.dataSource = _controller;
    view.delegate = _controller;
    view.separatorStyle = UITableViewCellSeparatorStyleNone;
    return view;
}

- (void) setData:(id) value withKey:(LynxRenderObjectAttr) attr; {
    UITableView *listView = SAFE_CONVERT(self.view, UITableView);
    switch (attr) {
        case SCROLL_LEFT:
            [listView setContentOffset:CGPointMake([SAFE_CONVERT(value, NSNumber) intValue], 0) animated:YES];
            break;
        case SCROLL_TOP:
            [listView setContentOffset:CGPointMake(0, [SAFE_CONVERT(value, NSNumber) intValue]) animated:YES];
            break;
        default:
            break;
    }
}

- (void)removeChild:(LynxRenderObjectImpl *)child {
    [self reloadData];
}

- (void)insertChild:(LynxRenderObjectImpl *)child atIndex:(int)index {
    [self reloadData];
}

- (void) reloadData {
    [_controller reloadData:self.renderObjectImpl];
}

- (void) addEventListener:(NSString *)event {
    if ([event isEqualToString:@"scroll"]) {
        self.controller.scrollEventEnable = true;
    }
}

- (void) removeEventListener:(NSString *)event {
    if ([event isEqualToString:@"scroll"]) {
        self.controller.scrollEventEnable = false;
    }
}
@end
