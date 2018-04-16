// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_UTILS_PIXEL_UTIL_H_
#define UI_UTILS_PIXEL_UTIL_H_

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface LxPixelUtil : NSObject

+ (CGFloat) pxToLynxNumber:(CGFloat) px;
+ (CGFloat) lynxNumberToPx:(CGFloat) number;

@end

#endif  // UI_UTILS_PIXEL_UTIL_H_
