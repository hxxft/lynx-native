// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LynxViewController.h"
#import "LynxView.h"
#import "LYXScreenUtil.h"

@interface LynxViewController ()
@property LynxView *lynxView;
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
    _lynxView = [[LynxView alloc] initWithFrame:frame];
    
    [self.view addSubview:_lynxView];

//    [_lynxView loadScriptFile:@"test"];
    [_lynxView loadPage:@"Assets://assets.bundle/Home/"];
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
    [_lynxView destroy];
}

@end
