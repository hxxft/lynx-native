// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>

@protocol NavigatorObjectProtocol <JSExport>

@property(nonatomic, strong, readonly) NSString *userAgent;
@property(nonatomic, strong, readonly) NSString *appCodeName;
@property(nonatomic, strong, readonly) NSString *appName;
@property(nonatomic, strong, readonly) NSString *platform;
@property(nonatomic, strong, readonly) NSString *appVersion;

@end

@interface NavigatorObject : NSObject<NavigatorObjectProtocol>

@end
