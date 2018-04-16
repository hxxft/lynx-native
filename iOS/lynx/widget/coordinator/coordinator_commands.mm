// Copyright 2017 The Lynx Authors. All rights reserved.

#include "widget/coordinator/coordinator_commands.h"
#include "widget/coordinator/coordinator_types.h"

#include "base/defines.h"

@implementation LxCrdCommands {
    NSMutableDictionary *_commands;
}

LX_NOT_IMPLEMENTED(- (instancetype) init)

- (instancetype)initWithContent:(NSString *) content {
    self = [super init];
    if (self) {
        _commands = [[NSMutableDictionary alloc] init];
        NSArray *commandGroup = [content componentsSeparatedByString:@"\\|"];
        for (NSString *command in commandGroup) {
            NSArray *details = [command componentsSeparatedByString:@":"];
            if (details.count != 2) continue;
            NSString *name = [details[0] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
            NSString *value = details[1];
            [_commands setObject:value forKey:name];
        }
    }
    return self;
}

- (NSString *)getCommand:(NSString *)type {
    return _commands[type];
}

@end
