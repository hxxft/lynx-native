// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/render_object_impl_bridge.h"

#include "widget/listview/lynx_ui_listview.h"

#include "base/ui_factory.h"
#include "base/ios/common.h"

#include "render/ios/render_object_impl_ios.h"


@implementation RenderObjectImplBridge

- (id) initWithRenderObjectImpl:(lynx::RenderObjectImplIOS*) impl withType:(lynx::RenderObjectType) type{
    self = [super init];
    if (self) {
        render_object_impl_ = impl;
        type_ = type;
        _attributes = [[NSMutableDictionary alloc] init];
        _data = [[NSMutableDictionary alloc] init];
        _events = [[NSMutableArray alloc] init];
        if (type == lynx::LYNX_BODY) {
            _root = self;
        }
    }
    return self;
}

- (void) updateStyle:(const lynx::CSSStyle&) style {
    style_ = style;
    if(_ui) {
        [_ui updateStyle:style];
    }
}

- (void) setPosition:(const base::Position&) position {
    position_ = position;
    if(_ui) {
        [_ui setPosition:position];
    }
}

- (void) setSize:(const base::Size&) size {
    size_ = size;
    if(_ui) {
        [_ui setSize:size];
    }
}

- (void) insertChild:(RenderObjectImplBridge *) child atIndex:(int) index {
    if(!child) return;
    if(!_children) {
        _children  = [[NSMutableArray alloc] init];
    }
    
    child.parent = self;
    
    if(index < 0) {
        [_children addObject:child];
    }else{
        _children[index] = child;
    }
    
    if (_root) {
        child.root = _root;
    }
    
    if(_ui) {
        if(!child.ui
           && render_object_impl_->render_object_type() != lynx::LYNX_LISTVIEW
           && render_object_impl_->render_object_type() != lynx::LYNX_LISTSHADOW) {
            [child createLynxUI];
        }
        [_ui insertChild:child atIndex:index];
    }
    
    if (render_object_impl_->render_object_type() == lynx::LYNX_LISTSHADOW
        && _parent != 0
        && _parent->render_object_impl_->render_object_type() == lynx::LYNX_LISTVIEW
        && _parent.ui) {
        
        LynxUI *listview = _parent.ui;
        [((LxUIListView *)listview) reloadData];
    }
}

- (void) removeChild:(RenderObjectImplBridge *) child {
    if (!_children) return;
    child.parent = nil;
    [_children removeObject:child];
    
    if (_ui) {
        [_ui removeChild:child];
    }
    
    if (render_object_impl_->render_object_type() == lynx::LYNX_LISTSHADOW
        && _parent != 0
        && _parent->render_object_impl_->render_object_type() == lynx::LYNX_LISTVIEW
        && _parent.ui) {
        LynxUI *listview = _parent.ui;
        [((LxUIListView *)listview) reloadData];
    }
}

- (void) setAttribute:(NSString *)value forKey:(NSString *) key; {
    [_attributes setObject:value forKey:key];
    if (_ui) {
        [_ui setAttribute:value forKey:key];
    }
}

- (void) requestLayout {
    if (_ui) {
        [_ui requestLayout];
    }
}

- (void) setData:(id) value withKey:(LynxRenderObjectAttr) attr; {
    [_data setObject:value forKey:[NSNumber numberWithInt:attr]];
    if (_ui) {
       [_ui setData:value withKey:attr];
    }
}

- (void) addEventListener:(NSString *) event {
    [_events addObject:event];
    if (_ui) {
        [_ui addEventListener:event];
    }
}

- (void) removeEventListener:(NSString *) event {
    [_events removeObject:event];
    if (_ui) {
        [_ui removeEventListener:event];
    }
}

- (void) dispatchEvent:(NSString *)event withParam:(NSArray *)array {
    render_object_impl_->DispatchEvent([event UTF8String], array);
}

- (void) updateData:(id)data withKey:(LynxRenderObjectAttr)key {
    render_object_impl_->UpdateData(key, data);
}

- (LynxUI *) createLynxUI {
    _ui = [LynxUIFactory createUI:self];
    for (RenderObjectImplBridge *child in _children) {
        [child createLynxUI];
        [_ui insertChild:child atIndex:-1];
    }
    return _ui;
}

- (void) setText:(NSString *) text {
    _text = text;
    if (_ui) {
       [_ui setText:text];
    }
}

- (void)setRoot:(RenderObjectImplBridge *)root {
    _root = root;
    if (_children) {
        for (RenderObjectImplBridge *child in _children) {
            [child setRoot:root];
        }
    }
}

@end
