// Copyright 2017 The Lynx Authors. All rights reserved.

#include "utils/pixel_util.h"

#include "utils/screen_util.h"

@implementation LxPixelUtil

const static int kLynxDefaultZoomRatioDp = 750;

+ (CGFloat) pxToLynxNumber:(CGFloat) px {
    // Origin: px / density * (zoomRatio / (screenWidth / density))
    return px * kLynxDefaultZoomRatioDp / [[LxScreenUtil shareInstance] getScreenWidth];
}

+ (CGFloat) lynxNumberToPx:(CGFloat) number {
    return number * [[LxScreenUtil shareInstance] getScreenWidth] / kLynxDefaultZoomRatioDp;
}

@end
