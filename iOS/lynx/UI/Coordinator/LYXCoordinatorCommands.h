// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>

@interface LYXCoordinatorCommands : NSObject

- (instancetype)initWithContent:(NSString *) content NS_DESIGNATED_INITIALIZER;
- (NSString *)getCommand:(NSString *) type;
@end
