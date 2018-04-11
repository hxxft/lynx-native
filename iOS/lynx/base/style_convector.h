// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_BASE_STYLE_CONVECTOR_H_
#define UI_BASE_STYLE_CONVECTOR_H_

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#include "layout/css_style.h"

@interface CSSStyleConvector : NSObject

+ (NSTextAlignment) ConvectToTextAlignment:(lynx::TextStyleType)text_align;
@end

#endif  // UI_BASE_STYLE_CONVECTOR_H_
