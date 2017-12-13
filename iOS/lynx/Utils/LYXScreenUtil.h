// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface LYXScreenUtil : NSObject

@property(nonatomic, readonly, getter = getScreenHeight) CGFloat screenHeight;
@property(nonatomic, readonly, getter = getScreenWidth) CGFloat screenWidth;
@property(nonatomic, readonly, getter = getScreenDensity) CGFloat screenDensity;

+ (instancetype) shareInstance;

@end
