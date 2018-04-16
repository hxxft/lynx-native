// Copyright 2017 The Lynx Authors. All rights reserved.
#ifndef UI_WIDGET_COORDINATOR_COORDINATOR_TYPES_H_
#define UI_WIDGET_COORDINATOR_COORDINATOR_TYPES_H_

#import <Foundation/Foundation.h>

@interface LxCrdTypes : NSObject
extern NSString * const kCoordinatorType_Scroll;
extern NSString * const kCoordinatorType_Touch;

@property(nonatomic, readonly) NSString *rawContent;

- (instancetype)initWithContent:(NSString *)content NS_DESIGNATED_INITIALIZER;

- (BOOL) hasType:(NSString *) type;

@end

#endif  // UI_WIDGET_COORDINATOR_COORDINATOR_TYPES_H_
