// Copyright 2017 The Lynx Authors. All rights reserved.

#include "widget/label/label_wrapper.h"

@implementation LabelWrapper

- (void)drawTextInRect:(CGRect)rect
{
    rect.size.height = [self sizeThatFits:rect.size].height;
    
    [super drawTextInRect:rect];
}

@end
