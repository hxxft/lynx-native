// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import "LynxUI.h"
#import "LynxRenderObjectAttr.h"

#include "layout/css_style.h"
#include "base/position.h"
#include "base/size.h"
#include "render/render_object_type.h"

namespace lynx {
    class RenderObjectImplIOS;
}

@interface LynxRenderObjectImpl : NSObject {
    
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
@property(nonatomic, readwrite) LynxRenderObjectImpl *root;
@property(nonatomic, readwrite) LynxRenderObjectImpl *parent;
@property(nonatomic, readwrite) LynxUI *ui;

- (id) initWithRenderObjectImpl:(lynx::RenderObjectImplIOS*) impl withType:(lynx::RenderObjectType) type;

- (void) updateStyle:(const lynx::CSSStyle&) style;

- (void) setPosition:(const base::Position&) position;

- (void) setSize:(const base::Size&) size;

- (void) insertChild:(LynxRenderObjectImpl *) child atIndex:(int) index;

- (void) removeChild:(LynxRenderObjectImpl *) child;

- (void) setAttribute:(NSString *)value forKey:(NSString *) key;

- (void) requestLayout;

- (void) setData:(id) value withKey:(LynxRenderObjectAttr) attr;

- (void) addEventListener:(NSString *) event;

- (void) removeEventListener:(NSString *) event;

// Should be called on JSThread
- (void) updateData:(id)data withKey:(LynxRenderObjectAttr)key;

// Should be called on JSThread
- (void) dispatchEvent:(NSString *)event withParam:(NSMutableArray *)array;

- (LynxUI *) createLynxUI;
@end
