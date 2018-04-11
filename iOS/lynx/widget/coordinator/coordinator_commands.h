// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_WIDGET_COORDINATOR_COORDINATOR_COMMANDS_H_
#define UI_WIDGET_COORDINATOR_COORDINATOR_COMMANDS_H_

#import <Foundation/Foundation.h>

@interface LYXCrdCommands : NSObject

- (instancetype)initWithContent:(NSString *) content NS_DESIGNATED_INITIALIZER;
- (NSString *)getCommand:(NSString *) type;
@end

#endif  // UI_WIDGET_COORDINATOR_COORDINATOR_COMMANDS_H_
