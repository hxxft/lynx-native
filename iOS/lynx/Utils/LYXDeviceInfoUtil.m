// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LYXDeviceInfoUtil.h"
#import <UIKit/UIKit.h>

@implementation LYXDeviceInfoUtil

static NSString *_deviceInfo;

+ (NSString *)deviceInfo {
    if (!_deviceInfo) {
        NSString *deviceModel = [[UIDevice currentDevice] model];
        NSString *sysVersion = [[[UIDevice currentDevice] systemVersion] stringByReplacingOccurrencesOfString:@"." withString:@"_"];
        
        _deviceInfo = [NSString stringWithFormat:@"%@; CPU iPhone OS %@ like Mac OS X", deviceModel, sysVersion];
    }
    
    return _deviceInfo;
}

@end
