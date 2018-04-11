// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_UTILS_SCREEN_UTIL_H_
#define UI_UTILS_SCREEN_UTIL_H_

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface LYXScreenUtil : NSObject

@property(nonatomic, readonly, getter = getScreenHeight) CGFloat screenHeight;
@property(nonatomic, readonly, getter = getScreenWidth) CGFloat screenWidth;
@property(nonatomic, readonly, getter = getScreenDensity) CGFloat screenDensity;

+ (instancetype) shareInstance;

@end

#endif  // UI_UTILS_SCREEN_UTIL_H_
