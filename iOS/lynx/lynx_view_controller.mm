// Copyright 2017 The Lynx Authors. All rights reserved.

#include "lynx_view_controller.h"
#include "widget/body/lynx_content_view.h"

#include "utils/screen_util.h"

@interface LynxViewController () {
@private LxContentView *content_view_;
}
@end

@implementation LynxViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.navigationItem.title = @"Lynx";
    
    // status bar
    CGFloat statusHeight = [[UIApplication sharedApplication] statusBarFrame].size.height;
    // navigation bar
    CGFloat navBarHeight = self.navigationController.navigationBar.frame.size.height;
    CGRect frame = CGRectMake(0, navBarHeight + statusHeight, [[LYXScreenUtil shareInstance] getScreenWidth], [[LYXScreenUtil shareInstance] getScreenHeight] - navBarHeight - statusHeight);
    content_view_ = [[LxContentView alloc] initWithFrame:frame];
    
    [self.view addSubview:content_view_];

//    [_lynxView loadScriptFile:@"test"];
    [content_view_ loadPage:@"Assets://assets.bundle/home/"];
//    NSString *url = @"http://localhost:8080/";
//    [_lynxView loadUrl:url];
    
    // fix UITableViewWrapperView and UITableView size differs with autolayout
    self.automaticallyAdjustsScrollViewInsets = NO;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (id) initWithName:(NSString*) name {
    self = [super init];
    if(self != nil) {
        self.page_name_ = name;
    }
    return self;
}

- (void)dealloc {
    [content_view_ destroy];
}

@end
