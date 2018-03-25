// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LYXPixelUtil.h"
#import "LYXScreenUtil.h"

@implementation LYXPixelUtil

const static int kLynxDefaultZoomRatioDp = 750;

+ (CGFloat) pxToLynxNumber:(CGFloat) px {
    // Origin: px / density * (zoomRatio / (screenWidth / density))
    return px * kLynxDefaultZoomRatioDp / [[LYXScreenUtil shareInstance] getScreenWidth];
}

+ (CGFloat) lynxNumberToPx:(CGFloat) number {
    return number * [[LYXScreenUtil shareInstance] getScreenWidth] / kLynxDefaultZoomRatioDp;
}

@end
