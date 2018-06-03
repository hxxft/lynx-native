// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <CoreTelephony/CTTelephonyNetworkInfo.h>
#import "AFNetworkReachabilityManager.h"

#import "plugin/impl/net_info/ios/net_info_plugin.h"

@implementation NetInfoPlugin


-(id) init {
    self = [super init];
    if(self != nil) {
        self.pluginName = @"NetInfo";
    }
    return self;
}

-(void) dealloc {
    [self stopNetworkStatusObserver];
}

-(void) getConnectInfo:(NSInteger)clientId method:(NSNumber*)methodId arguments:(NSArray*) args {
    __weak typeof(self) weakSelf = self;
 dispatch_sync(dispatch_get_main_queue(), ^{
     __strong __typeof(weakSelf)strongSelf = weakSelf;
    NSMutableArray* result = [[NSMutableArray alloc]init];
     [result addObject:[strongSelf networkingStatesFromStatebar] ? : @'unknown'];
     [strongSelf returnBack:clientId method:methodId successed:true argments:result];
 });
}

-(void) addEvent:(NSString*) event {
    if([event isEqualToString:@"connectionChange"]) {
        [self startNetworkStatusObserver:event];
    }
    
}

-(void) removeEvent:(NSString*) event {
    if([event isEqualToString:@"connectionChange"]) {
        [self stopNetworkStatusObserver];
    }
}

-(void) startNetworkStatusObserver:(NSString*)event {
    AFNetworkReachabilityManager *mgr = [AFNetworkReachabilityManager sharedManager];
     __weak typeof(self) weakSelf = self;
    [mgr setReachabilityStatusChangeBlock:^(AFNetworkReachabilityStatus status) {
        NSString* networkStatus = nil;
        switch (status) {
            case AFNetworkReachabilityStatusUnknown://未知网络
                networkStatus = @"unknown";
                break;
            case AFNetworkReachabilityStatusNotReachable://没有网络(断网)
                networkStatus = @"none";
                break;
            case AFNetworkReachabilityStatusReachableViaWWAN://手机自带网络
                networkStatus = [self getCellularStates];
                break;
            case AFNetworkReachabilityStatusReachableViaWiFi:// WIFI
                networkStatus = @"wifi";
                break;
            default:
                networkStatus = @"unknown";
                break;
        }
        __strong __typeof(weakSelf)strongSelf = weakSelf;
        NSMutableArray* result = [[NSMutableArray alloc]init];
        [result addObject:networkStatus];
        [strongSelf dispatchEvent:event argments:result];
        
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
