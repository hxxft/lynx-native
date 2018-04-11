// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_WIDGET_COORDINATOR_COORDINATOR_TREATMENT_H_
#define UI_WIDGET_COORDINATOR_COORDINATOR_TREATMENT_H_

#import <Foundation/Foundation.h>
#include "widget/coordinator/coordinator_cmd_executor.h"

@protocol LYXCrdResponder;
@class LYXCrdActionExecutor;

@interface LYXCrdTreatment : NSObject
extern NSString * const kAttrCoodinatorCommand;

@property(nonatomic, readwrite, copy) NSString *scrollCommand;
@property(nonatomic, readonly) id<LYXCrdResponder> responder;

- (instancetype)initWithResponder:(id<LYXCrdResponder>) responder actionExecutor:(LYXCrdActionExecutor *) executor NS_DESIGNATED_INITIALIZER;

- (void) addCoordinatorCommand:(NSString *) content;

- (void) initialize:(LYXCrdCommandExecutor *) executor;

- (void) reset;

- (void) onPropertiesUpdated:(LYXCrdCommandExecutor *) executor;

- (BOOL) onNestedActionWithType:(NSString *) type
                    andExecutor:(LYXCrdCommandExecutor *) executor
                        andArgs:(NSArray *) args ;
@end

#endif  // UI_WIDGET_COORDINATOR_COORDINATOR_TREATMENT_H_
