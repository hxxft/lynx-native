// Copyright 2017 The Lynx Authors. All rights reserved.

#include "label_measurer.h"

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

#include "base/size.h"
#include "layout/css_style.h"
#include "render/render_object.h"

namespace lynx {
    
    base::Size LabelMeasurer::MeasureLabelSize(RenderObject* render_object,
                                               const base::Size& size,
                                               const std::string& text){
        CSSStyle style = render_object->style_;
        NSString* label_text = [[NSString alloc]initWithUTF8String:text.c_str()];
        
        UIFont* ui_font = [UIFont systemFontOfSize:style.font_size_];
        NSMutableParagraphStyle* font_style = [[NSMutableParagraphStyle alloc] init];
        NSMutableDictionary* font_attribution = [[NSMutableDictionary alloc] init];
        
        NSInteger options = NSStringDrawingUsesFontLeading;
        options |= NSStringDrawingUsesLineFragmentOrigin;
        
        font_attribution[NSFontAttributeName] = ui_font;
        font_attribution[NSParagraphStyleAttributeName] = font_style;
        
        CGSize label_size;
        label_size.width = size.width_;
        label_size.height = size.height_;
        CGSize calc_size_out = [label_text boundingRectWithSize:label_size
                                options:options
                             attributes:font_attribution
                                context:nil].size;
        
        int line_count = calc_size_out.height / style.font_size_;
        
        if(style.text_white_space_ == CSSTEXT_WHITESPACE_NOWRAP) {
            calc_size_out.height = calc_size_out.height / line_count;
        }
        return base::Size(ceilf(calc_size_out.width), ceilf(calc_size_out.height));
    }
    
    base::Size LabelMeasurer::MeasureLabelSizeAndSetTextLayout(
                                               RenderObject* render_object,
                                               const base::Size& size,
                                               const std::string& text){
        return MeasureLabelSize(render_object, size, text);
    }
}
