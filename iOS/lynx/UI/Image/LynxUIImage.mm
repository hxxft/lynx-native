// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LynxUIImage.h"
#import "LynxImageDownloader.h"
#import "LynxUIView.h"

#include "base/ios/common.h"

@implementation LynxUIImage

- (UIView *)createView:(LynxRenderObjectImpl *)impl {
    return [[UIImageView alloc] init];
}

- (void) setAttribute:(NSString *)value forKey:(NSString *)key {
    [super setAttribute:value forKey:key];
    if (key.length == 0) return;
    if([key isEqualToString:@"src"]) {
        [self setImageUrl:value];
    }
}

- (void) updateStyle:(const lynx::CSSStyle&) style {
    [super updateStyle:style];
    if (!self.view) {
        return;
    }
    // scale type
    UIImageView* imageView = SAFE_CONVERT(self.view, UIImageView);
    imageView.clipsToBounds = YES;
    imageView.contentMode = UIViewContentModeScaleAspectFill;
    switch(style.css_object_fit_) {
        case lynx::CSSIMAGE_OBJECT_FIT_FILL:
            imageView.contentMode = UIViewContentModeScaleToFill;
            break;
        case lynx::CSSIMAGE_OBJECT_FIT_CONTAIN:
            imageView.contentMode = UIViewContentModeScaleAspectFit;
            break;
        case lynx::CSSIMAGE_OBJECT_FIT_COVER:
            imageView.contentMode = UIViewContentModeScaleAspectFill;
            break;
        default:
            break;
    }
}

- (void) setImageUrl:(NSString *) url {
    [[ImageDownLoader shareInstance] downloadImage: url
                                              then:^(UIImage* image){
                                                    [((UIImageView *) self.view) setImage:image];
                                                }];
}

@end

