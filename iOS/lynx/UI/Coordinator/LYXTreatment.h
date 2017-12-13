// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import "LYXCommandExecutor.h"

@class LynxUI;

@interface LYXTreatment : NSObject
extern NSString * const kAttrCoodinatorCommand;

@property(nonatomic, readwrite, copy) NSString *scrollCommand;
@property(nonatomic, readonly) LynxUI *ui;

- (instancetype)initWithUI:(LynxUI*) ui NS_DESIGNATED_INITIALIZER;

- (void) addCoordinatorCommand:(NSString *) content;

- (void) initialize:(LYXCommandExecutor *) executor;

- (void) onPropertiesUpdated:(LYXCommandExecutor *) executor;

- (BOOL) onNestedActionWithType:(NSString *) type
                    andExecutor:(LYXCommandExecutor *) executor
                        andArgs:(NSArray *) args ;
@end
