// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LYXPixelUtil.h"
#import "LYXScreenUtil.h"

@implementation LYXPixelUtil

const static int kLynxDefaultReferScreenWidth = 750;

+ (CGFloat) pxToLynxNumber:(CGFloat) px {
    // Origin: px / density * (LYNX_DEFAULT_REFER_SCREEN_WIDTH / (screenWidth / density))
    return px * kLynxDefaultReferScreenWidth / [[LYXScreenUtil shareInstance] getScreenWidth];
}

+ (CGFloat) lynxNumberToPx:(CGFloat) number {
    return number * [[LYXScreenUtil shareInstance] getScreenWidth] / kLynxDefaultReferScreenWidth;
}

@end
