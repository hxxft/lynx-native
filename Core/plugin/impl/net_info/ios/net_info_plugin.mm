//
//  net_info_plugin.m
//  lynx
//
//  Created by dli on 2018/5/20.
//  Copyright © 2018年 lynx. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <CoreTelephony/CTTelephonyNetworkInfo.h>
#import "AFNetworkReachabilityManager.h"

#include "plugin/impl/net_info/ios/net_info_plugin.h"

@implementation NetInfoPlugin


+(void) createWithManager: (plugin::PluginManager*)manager {
    [[NetInfoPlugin alloc] initWithManager:manager];
}

-(id) initWithManager: (plugin::PluginManager*)manager {
    self = [super initWithName:@"NetInfo" pluginManager:manager];
    if(self != nil) {
        self.networkStatus = @"unknown";
        [self startNetworkStatusObserver];
    }
    return self;
}

-(void) dealloc {
    [self stopNetworkStatusObserver];
}

-(void) getConnectInfo: (NSNumber*)methodId arguments:(NSArray*) args {
 dispatch_sync(dispatch_get_main_queue(), ^{
    NSMutableArray* result = [[NSMutableArray alloc]init];
    [result addObject:[self networkingStatesFromStatebar]];
    [self Return:methodId resultType:plugin::Plugin::ResultType_Success argments:result];
 });
}

-(void) startNetworkStatusObserver {
    AFNetworkReachabilityManager *mgr = [AFNetworkReachabilityManager sharedManager];
    [mgr setReachabilityStatusChangeBlock:^(AFNetworkReachabilityStatus status) {
        switch (status) {
            case AFNetworkReachabilityStatusUnknown://未知网络
                _networkStatus = @"unknown";
                break;
            case AFNetworkReachabilityStatusNotReachable://没有网络(断网)
                _networkStatus = @"none";
                break;
            case AFNetworkReachabilityStatusReachableViaWWAN://手机自带网络
                _networkStatus = [self getCellularStates];
                break;
            case AFNetworkReachabilityStatusReachableViaWiFi:// WIFI
                _networkStatus = @"wifi";
                break;
            default:
                _networkStatus = @"unknown";
                break;
        }
        
    }];
    [mgr startMonitoring];
}

-(void) stopNetworkStatusObserver {
    AFNetworkReachabilityManager *mgr = [AFNetworkReachabilityManager sharedManager];
    [mgr stopMonitoring];
}

- (NSString *)getCellularStates{
    if ([[[UIDevice currentDevice] systemVersion] compare:@"7.0" options:NSNumericSearch] != NSOrderedAscending) {
        CTTelephonyNetworkInfo *telephonyInfo = [CTTelephonyNetworkInfo new];
        if ([telephonyInfo.currentRadioAccessTechnology isEqualToString:CTRadioAccessTechnologyGPRS]) {
            return @"2g";
        } else if ([telephonyInfo.currentRadioAccessTechnology  isEqualToString:CTRadioAccessTechnologyEdge]) {
            return @"2g";
        } else if ([telephonyInfo.currentRadioAccessTechnology  isEqualToString:CTRadioAccessTechnologyWCDMA]) {
            return @"3g";
        } else if ([telephonyInfo.currentRadioAccessTechnology  isEqualToString:CTRadioAccessTechnologyHSDPA]) {
            return @"3g";
        } else if ([telephonyInfo.currentRadioAccessTechnology  isEqualToString:CTRadioAccessTechnologyHSUPA]) {
            return @"3g";
        } else if ([telephonyInfo.currentRadioAccessTechnology  isEqualToString:CTRadioAccessTechnologyCDMA1x]) {
            return @"3g";
        } else if ([telephonyInfo.currentRadioAccessTechnology  isEqualToString:CTRadioAccessTechnologyCDMAEVDORev0]) {
            return @"3g";
        } else if ([telephonyInfo.currentRadioAccessTechnology  isEqualToString:CTRadioAccessTechnologyCDMAEVDORevA]) {
            return @"3g";
        } else if ([telephonyInfo.currentRadioAccessTechnology  isEqualToString:CTRadioAccessTechnologyCDMAEVDORevB]) {
            return @"3g";
        } else if ([telephonyInfo.currentRadioAccessTechnology  isEqualToString:CTRadioAccessTechnologyeHRPD]) {
            return @"3g";
        } else if ([telephonyInfo.currentRadioAccessTechnology  isEqualToString:CTRadioAccessTechnologyLTE]) {
            return @"4g";
        }
    }
    return @"cellular";
}

- (NSString *)networkingStatesFromStatebar {
    // 状态栏是由当前app控制的，首先获取当前app
    UIApplication *app = [UIApplication sharedApplication];
    NSArray *children = [[[app valueForKeyPath:@"statusBar"] valueForKeyPath:@"foregroundView"] subviews];
    int type = 0;
    for (id child in children) {
        if ([child isKindOfClass:[NSClassFromString(@"UIStatusBarDataNetworkItemView") class]]) {
            type = [[child valueForKeyPath:@"dataNetworkType"] intValue];
        }
    }
    NSString *state = @"unknown";

    switch (type) {
        case 0:
            state = @"none";
            break;
        case 1:
            state = @"2G";
            break;
        case 2:
            state = @"3G";
            break;
        case 3:
            state = @"4G";
            break;
        case 4:
            state = @"4G";
            break;
        case 5:
            state = @"wifi";
            break;
        default:
            state = @"unknown";
            break;
    }

    return state;
}

@end
