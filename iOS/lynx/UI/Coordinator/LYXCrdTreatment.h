// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import "LYXCrdCommandExecutor.h"

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
