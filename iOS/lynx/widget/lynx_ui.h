// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_WIDGET_LYNX_UI_H_
#define UI_WIDGET_LYNX_UI_H_

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include "base/render_object_attr.h"
#include "widget/coordinator/coordinator_sponsor.h"
#include "widget/coordinator/coordinator_responder.h"

#include "layout/css_style.h"
#include "base/position.h"
#include "base/size.h"

@class RenderObjectImplBridge;
@class LxUIBody;

@interface LynxUI<__covariant V : UIView*> : NSObject<LxCrdSponsor, LxCrdResponder>

@property(nonatomic, readwrite, weak) RenderObjectImplBridge *renderObjectImpl;
@property(nonatomic, readwrite) V view;

@property(nonatomic, readwrite) CGFloat offsetTop;
@property(nonatomic, readwrite) CGFloat offsetLeft;
@property(nonatomic, readwrite) CGFloat offsetBottom;
@property(nonatomic, readwrite) CGFloat offsetRight;

- (id) initWithRenderObjectImpl:(RenderObjectImplBridge *) impl NS_DESIGNATED_INITIALIZER;

- (BOOL) isEmpty;

- (V) getView;

- (LxUIBody *) getRootUI;

- (V) createView:(RenderObjectImplBridge *) impl;

- (void) updateStyle:(const lynx::CSSStyle&) style;

- (void) setPosition:(const base::Position&) position;

- (void) updateFrame;

- (void) updateBounds;

- (void) setSize:(const base::Size&) size;

- (void) insertChild:(RenderObjectImplBridge *) child atIndex:(int) index;

- (void) removeChild:(RenderObjectImplBridge *) child;

- (void) setText:(NSString *) text;

- (void) setAttribute:(NSString *) value forKey:(NSString *) key NS_REQUIRES_SUPER;

- (void) requestLayout;

- (void) setData:(id) value withKey:(LynxRenderObjectAttr) attr;

- (void) addEventListener:(NSString *) event;

- (void) removeEventListener:(NSString *) event;

- (void) bindRenderObjectImpl:(RenderObjectImplBridge *) impl;

- (void) unbindRenderObjectImpl;

- (void) setAttribution:(RenderObjectImplBridge *) impl;

- (void) addEvents:(RenderObjectImplBridge *) impl;

- (void) postEvent:(NSString *)event withValue:(NSArray *)array;

- (void) postData:(id)data withKey:(LynxRenderObjectAttr)key;

@end

#endif  // UI_WIDGET_LYNX_UI_H_
