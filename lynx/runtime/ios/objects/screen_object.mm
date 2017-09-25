// Copyright 2017 The Lynx Authors. All rights reserved.

#import "screen_object.h"

@implementation ScreenObject

-(CGFloat) width
{
    return [[UIScreen mainScreen] bounds].size.width;
}

-(CGFloat) height
{
    return [[UIScreen mainScreen] bounds].size.height;
}

@end
