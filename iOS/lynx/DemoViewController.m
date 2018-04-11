// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import "DemoViewController.h"
#include "lynx_view_controller.h"

@interface DemoViewController ()

@end

@implementation DemoViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.navigationItem.title = @"首页";
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)toNewStoryBoard:(id)sender {
    UIViewController *vc = [[LynxViewController alloc] init];
    [self.navigationController pushViewController:vc animated:YES];
}

@end
