// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_BASE_UI_ACTION_H_
#define UI_BASE_UI_ACTION_H_

#import <Foundation/Foundation.h>

#include "base/render_object_impl_bridge.h"

typedef enum {
    DO_EVENT_NONE,
    DO_EVENT_ACTION,
    DO_UPDATE_DATA_ACTION
} LynxUIActionType;

@interface LynxUIAction : NSObject

@property(nonatomic, readwrite) RenderObjectImplBridge *renderObjectImpl;
@property LynxUIActionType type;

- (id) initWithType:(LynxUIActionType)type andTarget:(RenderObjectImplBridge *)target;

- (void) doAction;
@end

#endif  // UI_BASE_UI_ACTION_H_
