//
//  main.m
//  unittests
//
//  Created by dli on 2018/5/2.
//  Copyright © 2018年 dli. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"

#include "gtest/gtest.h"

int main(int argc, char * argv[]) {
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}
