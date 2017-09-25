// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LynxViewController.h"
#import "LynxView.h"

@interface LynxViewController ()
@property LynxView *lynxView;
@end

@implementation LynxViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.navigationItem.title = @"Lynx";
    
    CGRect screenRect = [UIScreen mainScreen].bounds;
    // status bar
    CGFloat statusHeight = [[UIApplication sharedApplication] statusBarFrame].size.height;
    // navigation bar
    CGFloat navBarHeight = self.navigationController.navigationBar.frame.size.height;
    CGRect frame = CGRectMake(0, navBarHeight + statusHeight, screenRect.size.width, screenRect.size.height - navBarHeight - statusHeight);
    _lynxView = [[LynxView alloc] initWithFrame:frame];
    
    [self.view addSubview:_lynxView];

    //[_lynxView loadScriptFile:@"test"];
    [_lynxView loadHTMLFile:@"index"];
//    NSString *url = @"http://localhost:8080/";
//    [_lynxView loadUrl:url];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc {
    [_lynxView destroy];
}

@end
