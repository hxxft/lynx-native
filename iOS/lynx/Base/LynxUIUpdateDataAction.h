// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import "LynxUIAction.h"
#import "LynxRenderObjectAttr.h"

@interface LynxUIUpdateDataAction : LynxUIAction

@property(nonatomic, readwrite) LynxRenderObjectAttr key;
@property(nonatomic, readwrite) id data;

- (id) initWithTarget:(LynxRenderObjectImpl *)target andKey:(LynxRenderObjectAttr)key andData:(id)data;
@end
