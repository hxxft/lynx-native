// Copyright 2017 The Lynx Authors. All rights reserved.

#include "utils/device_info_util.h"

#import <UIKit/UIKit.h>

@implementation LxDeviceInfoUtil

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
