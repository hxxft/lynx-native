// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LynxUISlider.h"
#include "base/ios/common.h"

static const NSString *kAttrMin = @"min";
static const NSString *kAttrMax = @"max";
static const NSString *kAttrValue = @"value";
static const NSString *kAttrStep = @"step";
static const NSString *kAttrActiveLineColor = @"active-line-color";
static const NSString *kAttrBackgroundLineColor = @"background-line-color";
static const NSString *kEventChange = @"change";

static const NSInteger kDefaultMax = 100;
static const NSInteger kDefaultMin = 0;

@interface LynxUISlider()

@property(nonatomic) NSInteger max;
@property(nonatomic) NSInteger min;
@property(nonatomic) NSInteger step;

@end

@implementation LynxUISlider

- (UIView *)createView:(LynxRenderObjectImpl *)impl {
    UISlider *view = [[UISlider alloc] init];
    view.clipsToBounds = YES;
    view.continuous = YES;
    return view;
}

- (void)setAttribute:(NSString *)value forKey:(NSString *)key {
    UISlider *sliderView = SAFE_CONVERT(self.view, UISlider);
    if ([kAttrMax isEqualToString:key]) {
        sliderView.maximumValue = value.floatValue;
    } else if ([kAttrMin isEqualToString:key]) {
        sliderView.minimumValue = value.floatValue;
    } else if ([kAttrValue isEqualToString:key]) {
        [sliderView setValue:value.floatValue animated:NO];
    } else if ([kAttrStep isEqualToString:key]) {
        _step = value.integerValue;
    } else if ([kAttrActiveLineColor isEqualToString:key]) {
        
    } else if ([kAttrBackgroundLineColor isEqualToString:key]) {
        
    }
}

- (void)addEventListener:(NSString *)event {
    UISlider *sliderView = SAFE_CONVERT(self.view, UISlider);
    if ([kEventChange isEqualToString:event]) {
        [sliderView addTarget:self action:@selector(valueChanged:) forControlEvents:UIControlEventValueChanged];
    }
}

- (void)removeEventListener:(NSString *)event {
    if ([kEventChange isEqualToString:event]) {
        
    }
}

- (void)valueChanged:(UISlider *)sender {
    UISlider *sliderView = SAFE_CONVERT(self.view, UISlider);
    NSInteger index = (NSInteger)(sliderView.value + 0.5);
//    [sliderView setValue:index animated:NO];
}

@end
