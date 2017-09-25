// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include "css_style.h"

@interface CSSStyleConvector : NSObject

+ (NSTextAlignment) ConvectToTextAlignment:(lynx::TextStyleType)text_align;
@end
