// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_CONTENT_BASE_RENDER_OBJECT_IMPL_BRIDGE_H_
#define LYNX_CONTENT_BASE_RENDER_OBJECT_IMPL_BRIDGE_H_

#import <Foundation/Foundation.h>
#include "widget/lynx_ui.h"

#include "base/render_object_attr.h"

#include "layout/css_style.h"
#include "base/position.h"
#include "base/size.h"
#include "render/render_object_type.h"

namespace lynx {
    class RenderObjectImplIOS;
}

@interface RenderObjectImplBridge : NSObject {
    
    @public
    base::Position position_;
    base::Size size_;
    lynx::CSSStyle style_;
    lynx::RenderObjectType type_;
    
    @private
    lynx::RenderObjectImplIOS* render_object_impl_;
    
}

@property(nonatomic, readwrite) NSString *text;
@property(nonatomic, readwrite) NSMutableDictionary *attributes;
@property(nonatomic, readwrite) NSMutableDictionary *data;
@property(nonatomic, readwrite) NSMutableArray *events;
@property(nonatomic, readwrite) NSMutableArray *children;
@property(nonatomic, readwrite) RenderObjectImplBridge *root;
@property(nonatomic, readwrite) RenderObjectImplBridge *parent;
@property(nonatomic, readwrite) LynxUI *ui;

- (id) initWithRenderObjectImpl:(lynx::RenderObjectImplIOS*) impl withType:(lynx::RenderObjectType) type;

- (void) updateStyle:(const lynx::CSSStyle&) style;

- (void) setPosition:(const base::Position&) position;

- (void) setSize:(const base::Size&) size;

- (void) insertChild:(RenderObjectImplBridge *) child atIndex:(int) index;

- (void) removeChild:(RenderObjectImplBridge *) child;

- (void) setAttribute:(NSString *)value forKey:(NSString *) key;

- (void) requestLayout;

- (void) setData:(id) value withKey:(LynxRenderObjectAttr) attr;

- (void) addEventListener:(NSString *) event;

- (void) removeEventListener:(NSString *) event;

// Should be called on JSThread
- (void) updateData:(id)data withKey:(LynxRenderObjectAttr)key;

// Should be called on JSThread
- (void) dispatchEvent:(NSString *)event withParam:(NSArray *)array;

- (LynxUI *) createLynxUI;
@end

#endif  // LYNX_CONTENT_BASE_RENDER_OBJECT_IMPL_BRIDGE_H_
