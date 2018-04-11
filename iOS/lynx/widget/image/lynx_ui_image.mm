// Copyright 2017 The Lynx Authors. All rights reserved.

#include "widget/image/lynx_ui_image.h"

#include "base/ios/common.h"
#include "widget/image/image_downloader.h"
#include "widget/view/lynx_ui_view.h"

@implementation LxUIImage

- (id)createView:(LynxRenderObjectImpl *)impl {
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
    self.view.clipsToBounds = YES;
    self.view.contentMode = UIViewContentModeScaleAspectFill;
    switch(style.css_object_fit_) {
        case lynx::CSSIMAGE_OBJECT_FIT_FILL:
            self.view.contentMode = UIViewContentModeScaleToFill;
            break;
        case lynx::CSSIMAGE_OBJECT_FIT_CONTAIN:
            self.view.contentMode = UIViewContentModeScaleAspectFit;
            break;
        case lynx::CSSIMAGE_OBJECT_FIT_COVER:
            self.view.contentMode = UIViewContentModeScaleAspectFill;
            break;
        default:
            break;
    }
}

- (void) setImageUrl:(NSString *) url {
    NSLog(@"%@\n", url);
    [[ImageDownLoader shareInstance] downloadImage: url
                                              then:^(UIImage* image){
                                                    [((UIImageView *) self.view) setImage:image];
                                                }];
}

@end

