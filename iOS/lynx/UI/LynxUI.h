// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "LynxRenderObjectAttr.h"
#import "LYXCrdSponsor.h"
#import "LYXCrdResponder.h"

#include "layout/css_style.h"
#include "base/position.h"
#include "base/size.h"

@class LynxRenderObjectImpl;
@class LynxUIBody;

@interface LynxUI<__covariant V : UIView*> : NSObject<LYXCrdSponsor, LYXCrdResponder>

@property(nonatomic, readwrite) LynxRenderObjectImpl *renderObjectImpl;
@property(nonatomic, readwrite) V view;

@property(nonatomic, readwrite) CGFloat offsetTop;
@property(nonatomic, readwrite) CGFloat offsetLeft;
@property(nonatomic, readwrite) CGFloat offsetBottom;
@property(nonatomic, readwrite) CGFloat offsetRight;

- (id) initWithRenderObjectImpl:(LynxRenderObjectImpl *) impl NS_DESIGNATED_INITIALIZER;

- (BOOL) isEmpty;

- (V) getView;

- (LynxUIBody *) getRootUI;

- (V) createView:(LynxRenderObjectImpl *) impl;

- (void) updateStyle:(const lynx::CSSStyle&) style;

- (void) setPosition:(const base::Position&) position;

- (void) updateFrame;

- (void) updateBounds;

- (void) setSize:(const base::Size&) size;

- (void) insertChild:(LynxRenderObjectImpl *) child atIndex:(int) index;

- (void) removeChild:(LynxRenderObjectImpl *) child;

- (void) setText:(NSString *) text;

- (void) setAttribute:(NSString *) value forKey:(NSString *) key NS_REQUIRES_SUPER;

- (void) requestLayout;

- (void) setData:(id) value withKey:(LynxRenderObjectAttr) attr;

- (void) addEventListener:(NSString *) event;

- (void) removeEventListener:(NSString *) event;

- (void) bindRenderObjectImpl:(LynxRenderObjectImpl *) impl;

- (void) unbindRenderObjectImpl;

- (void) setAttribution:(LynxRenderObjectImpl *) impl;

- (void) addEvents:(LynxRenderObjectImpl *) impl;

- (void) postEvent:(NSString *)event withValue:(NSArray *)array;

- (void) postData:(id)data withKey:(LynxRenderObjectAttr)key;

@end
