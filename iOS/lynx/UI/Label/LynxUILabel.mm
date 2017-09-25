// Copyright 2017 The Lynx Authors. All rights reserved.

#import <CoreGraphics/CGGeometry.h>
#import "LynxStyleConvector.h"
#import "LynxUILabel.h"
#import "IOSLabel.h"
#import "LynxRenderObjectImpl.h"

#include "base/ios/common.h"

@implementation LynxUILabel

- (UIView *)createView:(LynxRenderObjectImpl *)impl {
    UIView *view = [[IOSLabel alloc] init];
    view.clipsToBounds = YES;
    return view;
}

- (void)updateStyle:(const lynx::CSSStyle &)style {
    if (!self.view) {
        return;
    }
    UILabel* label = SAFE_CONVERT(self.view, UILabel);
    self.view.backgroundColor = [UIColor clearColor];
    self.view.alpha = style.opacity_;
    label.textColor = COLOR_CONVERT(style.font_color_);
    label.textAlignment = [CSSStyleConvector ConvectToTextAlignment:style.text_align_];
    if (style.font_weight_ == lynx::CSSTEXT_FONT_WEIGHT_BOLD) {
        label.font = [UIFont fontWithName:@ "Arial Rounded MT Bold"  size:(style.font_size_)];
    } else {
        label.font = [UIFont systemFontOfSize:style.font_size_];
    }
}

- (void)setText:(NSString *)text {
    ((UILabel *)self.view).text = text;
}

- (void)bindRenderObjectImpl:(LynxRenderObjectImpl *)impl {
    [super bindRenderObjectImpl:impl];
    [self setText:impl.text];
}

@end
