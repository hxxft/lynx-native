// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import "LynxUIAction.h"

@interface LynxUIEventAction : LynxUIAction

@property(nonatomic, readwrite) NSString *event;
@property(nonatomic, readwrite) NSArray *array;

- (id) initWithTarget:(LynxRenderObjectImpl *)target andEvent:(NSString *)event andValue:(NSArray *)array;
@end
