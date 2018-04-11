// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_BASE_UI_UPDATE_DATA_ACTION_H_
#define UI_BASE_UI_UPDATE_DATA_ACTION_H_

#import <Foundation/Foundation.h>
#include "base/ui_action.h"
#include "base/render_object_attr.h"

@interface LynxUIUpdateDataAction : LynxUIAction

@property(nonatomic, readwrite) LynxRenderObjectAttr key;
@property(nonatomic, readwrite) id data;

- (id) initWithTarget:(LynxRenderObjectImpl *)target andKey:(LynxRenderObjectAttr)key andData:(id)data;
@end

#endif  // UI_BASE_UI_UPDATE_DATA_ACTION_H_
