// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import "LynxUIAction.h"

@interface LynxUIEventAction : LynxUIAction

@property(nonatomic, readwrite) NSString *event;
@property(nonatomic, readwrite) NSMutableArray *array;

- (id) initWithTarget:(LynxRenderObjectImpl *)target andEvent:(NSString *)event andValue:(NSMutableArray *)array;
@end
