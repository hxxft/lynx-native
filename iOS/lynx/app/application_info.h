// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_APP_APPLICATION_INFO_H_
#define UI_APP_APPLICATION_INFO_H_

#import <Foundation/Foundation.h>

@interface LynxApplicationInfo : NSObject

@property (readonly, nonatomic) NSString* packageName;
@property (readonly, nonatomic) NSString* mainPage;
@property (readonly, nonatomic) NSMutableArray* pages;
@property (readonly, nonatomic) BOOL debugable;

- (id) initWithManifest:(NSDictionary*)manifest;

@end

#endif  // UI_APP_APPLICATION_INFO_H_

