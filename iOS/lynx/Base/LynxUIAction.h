// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import "LynxRenderObjectImpl.h"

typedef enum {
    DO_EVENT_NONE,
    DO_EVENT_ACTION,
    DO_UPDATE_DATA_ACTION
} LynxUIActionType;

@interface LynxUIAction : NSObject

@property(nonatomic, readwrite) LynxRenderObjectImpl *renderObjectImpl;
@property LynxUIActionType type;

- (id) initWithType:(LynxUIActionType)type andTarget:(LynxRenderObjectImpl *)target;

- (void) doAction;
@end
