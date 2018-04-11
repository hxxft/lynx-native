// Copyright 2017 The Lynx Authors. All rights reserved.
#include "widget/label/lynx_ui_label.h"

#import <CoreGraphics/CGGeometry.h>

#include "base/style_convector.h"
#include "base/lynx_render_object_impl.h"
#include "base/ios/common.h"

@implementation LxUILabel

- (id)createView:(LynxRenderObjectImpl *)impl {
    UIView *view = [[LabelWrapper alloc] init];
    view.clipsToBounds = YES;
    return view;
}

- (void)updateStyle:(const lynx::CSSStyle &)style {
    if (!self.view) {
        return;
    }
    self.view.backgroundColor = [UIColor clearColor];
    self.view.alpha = style.opacity_;
    self.view.textColor = COLOR_CONVERT(style.font_color_);
    self.view.textAlignment = [CSSStyleConvector ConvectToTextAlignment:style.text_align_];
    if (style.font_weight_ == lynx::CSSTEXT_FONT_WEIGHT_BOLD) {
        self.view.font = [UIFont boldSystemFontOfSize:style.font_size_];
    } else {
        self.view.font = [UIFont systemFontOfSize:style.font_size_];
    }
}

- (void)setText:(NSString *)text {
    (self.view).text = text;
}

- (void)bindRenderObjectImpl:(LynxRenderObjectImpl *)impl {
    [super bindRenderObjectImpl:impl];
    [self setText:impl.text];
}

@end
