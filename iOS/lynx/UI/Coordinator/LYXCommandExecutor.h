// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>

#include "render/coordinator/coordinator_action.h"

@interface LYXCommandExecutor : NSObject

@property(nonatomic, copy) NSString *executable;

- (id) initWithExecutableContent:(NSString *) content;

- (void) updateExecutableContent:(NSString *) executable;

- (lynx::CoordinatorAction) executeCommandWithMethod:(NSString *) method
                                              andTag:(NSString *) tag
                                             andArgs:(double *) args
                                           andLength:(int) length;

- (BOOL) updateProperty:(NSString *) property withStr:(NSString *) value;
- (BOOL) updateProperty:(NSString *) property withBool:(BOOL) value;
- (BOOL) updateProperty:(NSString *) property withDouble:(double) value;

@end
