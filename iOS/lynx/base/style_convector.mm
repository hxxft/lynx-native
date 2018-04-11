// Copyright 2017 The Lynx Authors. All rights reserved.

#import "base/style_convector.h"

@implementation CSSStyleConvector

+ (NSTextAlignment) ConvectToTextAlignment:(lynx::TextStyleType)text_align {
    NSTextAlignment alignment = NSTextAlignment::NSTextAlignmentLeft;
    if (text_align == lynx::TextStyleType::CSSTEXT_ALIGN_LEFT) {
        alignment = NSTextAlignment::NSTextAlignmentLeft;
    } else if (text_align == lynx::TextStyleType::CSSTEXT_ALIGN_RIGHT) {
        alignment = NSTextAlignment::NSTextAlignmentRight;
    } else if (text_align == lynx::TextStyleType::CSSTEXT_ALIGN_RIGHT) {
        alignment = NSTextAlignment::NSTextAlignmentCenter;
    }
    return alignment;
}
@end
