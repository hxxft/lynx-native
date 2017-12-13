// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>

@interface LYXCoordinatorTypes : NSObject
extern NSString * const kCoordinatorType_Scroll;
extern NSString * const kCoordinatorType_Touch;

- (instancetype)initWithContent:(NSString *)content NS_DESIGNATED_INITIALIZER;

- (BOOL) hasType:(NSString *) type;

@end
