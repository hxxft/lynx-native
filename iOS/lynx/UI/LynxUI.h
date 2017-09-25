// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "LynxRenderObjectAttr.h"

#include "layout/css_style.h"
#include "base/position.h"
#include "base/size.h"

@class LynxRenderObjectImpl;
@class LynxUIBody;

@interface LynxUI : NSObject

@property(nonatomic, readwrite) LynxRenderObjectImpl *renderObjectImpl;
@property(nonatomic, readwrite) UIView *view;

- (id) initWithRenderObjectImpl:(LynxRenderObjectImpl *) impl;

- (BOOL) isEmpty;

- (UIView *) getView;

- (LynxUIBody *) getRootUI;

- (UIView *) createView:(LynxRenderObjectImpl *) impl;

- (void) updateStyle:(const lynx::CSSStyle&) style;

- (void) setPosition:(const base::Position&) position;

- (void) setSize:(const base::Size&) size;

- (void) insertChild:(LynxRenderObjectImpl *) child atIndex:(int) index;

- (void) removeChild:(LynxRenderObjectImpl *) child;

- (void) setText:(NSString *) text;

- (void) setAttribute:(NSString *) value forKey:(NSString *) key;

- (void) requestLayout;

- (void) setData:(id) value withKey:(LynxRenderObjectAttr) attr;

- (void) addEventListener:(NSString *) event;

- (void) removeEventListener:(NSString *) event;

- (void) bindRenderObjectImpl:(LynxRenderObjectImpl *) impl;

- (void) unbindRenderObjectImpl;

- (void) setAttribution:(LynxRenderObjectImpl *) impl;

- (void) addEvents:(LynxRenderObjectImpl *) impl;

- (void) postEvent:(NSString *)event withValue:(NSMutableArray *)array;

- (void) postData:(id)data withKey:(LynxRenderObjectAttr)key;

@end
