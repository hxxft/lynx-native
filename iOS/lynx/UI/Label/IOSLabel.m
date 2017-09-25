// Copyright 2017 The Lynx Authors. All rights reserved.

#import "IOSLabel.h"

@implementation IOSLabel

- (void)drawTextInRect:(CGRect)rect
{
    rect.size.height = [self sizeThatFits:rect.size].height;
    
    [super drawTextInRect:rect];
}

@end
