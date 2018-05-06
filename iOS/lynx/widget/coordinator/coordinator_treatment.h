// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_WIDGET_COORDINATOR_COORDINATOR_TREATMENT_H_
#define UI_WIDGET_COORDINATOR_COORDINATOR_TREATMENT_H_

#import <Foundation/Foundation.h>
#include "widget/coordinator/coordinator_cmd_executor.h"

@protocol LxCrdResponder;
@class LxCrdActionExecutor;

@interface LxCrdTreatment : NSObject
extern NSString * const kAttrCoodinatorCommand;

@property(nonatomic, readwrite, copy) NSString *scrollCommand;
@property(nonatomic, readonly, weak) id<LxCrdResponder> responder;

- (instancetype)initWithResponder:(id<LxCrdResponder>) responder actionExecutor:(LxCrdActionExecutor *) executor NS_DESIGNATED_INITIALIZER;

- (void) addCoordinatorCommand:(NSString *) content;

- (void) initialize:(LxCrdCommandExecutor *) executor;

- (void) reset;

- (void) onPropertiesUpdated:(LxCrdCommandExecutor *) executor;

- (BOOL) onNestedActionWithType:(NSString *) type
                    andExecutor:(LxCrdCommandExecutor *) executor
                        andArgs:(NSArray *) args ;
@end

#endif  // UI_WIDGET_COORDINATOR_COORDINATOR_TREATMENT_H_
