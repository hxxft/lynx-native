// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LynxUI.h"
#import "LynxUIBody.h"
#import "LynxUIEventAction.h"
#import "LynxUIUpdateDataAction.h"

#include "base/ios/common.h"
#include "render/render_object_type.h"
#include "render/ios/render_object_impl_ios.h"

@implementation LynxUI

- (id)initWithRenderObjectImpl:(LynxRenderObjectImpl *) impl {
    self = [super init];
    if (self) {
        _view = [self createView:impl];
        [self bindRenderObjectImpl:impl];
    }
    return self;
}

- (BOOL) isEmpty {
    return _view == nil;
}

- (UIView *) getView {
    return _view;
}

- (UIView *) createView:(LynxRenderObjectImpl *) impl {
    @throw [NSException exceptionWithName:NSInternalInconsistencyException
                                   reason:[NSString stringWithFormat:@"You must override %@ in a subclass", NSStringFromSelector(_cmd)]
                                 userInfo:nil];
}

- (void) updateStyle:(const lynx::CSSStyle&) style {
    if (!self.view) {
        return;
    }
    self.view.backgroundColor = COLOR_CONVERT(style.background_color_);
    self.view.alpha = style.opacity_;
    // Border radius setting
    if (style.border_radius_ > 0) {
        self.view.layer.cornerRadius = style.border_radius_ ;
        self.view.layer.masksToBounds = YES;
    } else {
        self.view.layer.cornerRadius = 0;
    }
    // Border width setting
    if (style.border_width_ > 0) {
        self.view.layer.borderColor = [COLOR_CONVERT(style.border_color_) CGColor];
        self.view.layer.borderWidth = style.border_width_;
    }
}

- (void) setPosition:(const base::Position&) position {
    _renderObjectImpl->position_ = position;
    if(_renderObjectImpl->position_.IsEmpty() || !_view) return;
    _view.frame = CGRectMake(_renderObjectImpl->position_.left_,
                                      _renderObjectImpl->position_.top_,
                                      _renderObjectImpl->position_.GetWidth(),
                                      _renderObjectImpl->position_.GetHeight());
}

- (void) setSize:(const base::Size &) size {
    
}

- (void) insertChild:(LynxRenderObjectImpl *) child atIndex:(int) index {
    
}

- (void) removeChild:(LynxRenderObjectImpl *) child {
    
}

- (void) setText:(NSString *) text {
    
}

- (void) setAttribute:(NSString *) value forKey:(NSString *) key {
    
}

- (void) requestLayout {
    
}

- (void) setData:(id) value withKey:(LynxRenderObjectAttr) attr {
    
}

- (void) addEventListener:(NSString *) event {
    
}

- (void) removeEventListener:(NSString *) event {
    
}

- (void) bindRenderObjectImpl:(LynxRenderObjectImpl *) impl {
    _renderObjectImpl = impl;
    [self updateStyle:impl->style_];
    [self setPosition:impl->position_];
    [self setAttribution:impl];
    [self addEvents:impl];
}

- (void) setAttribution:(LynxRenderObjectImpl *) impl {
    if (impl.attributes.count != 0) {
        for (id key in impl.attributes) {
            id value = impl.attributes[key];
            [self setAttribute:SAFE_CONVERT(value, NSString)
                        forKey:SAFE_CONVERT(key, NSString)];
        }
    }
}

- (void) addEvents:(LynxRenderObjectImpl *) impl {
    if (impl.events.count != 0) {
        for (id key in impl.events) {
            [self addEventListener:SAFE_CONVERT(key, NSString)];
        }
    }
}

- (void) unbindRenderObjectImpl {
    _renderObjectImpl = nil;
}

- (LynxUIBody *) getRootUI {
    if (_renderObjectImpl && _renderObjectImpl.root) {
        return (LynxUIBody *)_renderObjectImpl.root.ui;
    }
    return nil;
}

- (void) postEvent:(NSString *)event withValue:(NSMutableArray *)array {
    LynxUIBody *body = [self getRootUI];
    if (body) {
        [body collectAction:[[LynxUIEventAction alloc] initWithTarget:_renderObjectImpl andEvent:event andValue:array]];
    }
}

- (void) postData:(id)data withKey:(LynxRenderObjectAttr)key {
    LynxUIBody *body = [self getRootUI];
    if (body) {
        [body collectAction:[[LynxUIUpdateDataAction alloc] initWithTarget:_renderObjectImpl andKey:key andData:data]];
    }
}

@end