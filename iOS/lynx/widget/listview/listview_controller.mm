// Copyright 2017 The Lynx Authors. All rights reserved.

#include "widget/listview/listview_controller.h"
#include "base/render_object_attr.h"

@implementation LynxListViewController

- (instancetype) init:(UITableView *)tableView withUI:(LynxUI *)ui
{
    self = [super init];
    if (self) {
        _tableView = tableView;
        _ui = ui;
        _scrollEventEnable = false;
        _children = [[NSMutableArray alloc] init];
    }
    return self;
}

- (void)reloadData:(RenderObjectImplBridge *)impl {
    [_children removeAllObjects];
    [self buildChidrenImpl:impl];
    [self.tableView reloadData];
}

-(void)buildChidrenImpl:(RenderObjectImplBridge *)impl
{
    for (RenderObjectImplBridge *child in impl.children) {
        if (child->type_ == lynx::LYNX_LISTSHADOW) {
            [self buildChidrenImpl:child];
        } else {
            [_children addObject:child];
        }
    }
}

-(NSInteger) tableView:(nonnull UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return _children.count;
}

-(UITableViewCell *) tableView:(UITableView *)tableView cellForRowAtIndexPath:(nonnull NSIndexPath *)indexPath {
    UITableViewCell *cell = [[UITableViewCell alloc] init];
    // 设置listview的cell背景透明
    cell.backgroundColor = [UIColor clearColor];
    cell.contentView.backgroundColor = [UIColor clearColor];
    // 去除press后的颜色变化
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    cell.contentView.clipsToBounds = YES;
    cell.clipsToBounds = YES;
    NSInteger row = [indexPath row];
    RenderObjectImplBridge *element = _children[row];
    UIView * childView = [self buildChildView:element];
    [cell.contentView addSubview:childView];
    return cell;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    NSInteger row = [indexPath row];
    RenderObjectImplBridge *impl = _children[row];
    CGFloat height = impl->position_.GetHeight();
    return height;
}

- (UIView *) buildChildView:(RenderObjectImplBridge *)impl {
    if (!impl.ui) {
        [impl createLynxUI];
    }
    UIView * view = impl.ui.view;
    return view;
}

- (void)scrollViewWillBeginDragging:(UIScrollView *)scrollView {
}

- (void) scrollViewDidScroll:(UIScrollView *)scrollView {
    [_ui postData:[NSNumber numberWithUnsignedInteger:scrollView.contentOffset.y]
          withKey: LynxRenderObjectAttr::SCROLL_TOP];
    if (self.scrollEventEnable) {
        NSMutableArray *array = [[NSMutableArray alloc] init];
        NSMutableDictionary *event = [[NSMutableDictionary alloc] init];
        [event setValue:@"scroll" forKey:@"type"];
        array[0] = event;
        [_ui postEvent:@"scroll" withValue:array];
    }
    [_ui dispatchCoordinatorScrollTop:scrollView.contentOffset.y andLeft:0];
}

@end
