// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>

@interface LynxApplicationInfo : NSObject

@property (readonly, nonatomic) NSString* packageName;
@property (readonly, nonatomic) NSString* mainPage;
@property (readonly, nonatomic) NSMutableArray* pages;

- (id) initWithManifest:(NSDictionary*)manifest;

@end

