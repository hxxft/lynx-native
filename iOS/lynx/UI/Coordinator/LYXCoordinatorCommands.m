// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LYXCoordinatorCommands.h"
#import "LYXCoordinatorTypes.h"
#import "LYXDefines.h"

@implementation LYXCoordinatorCommands {
    NSMutableDictionary *_commands;
}

LYX_NOT_IMPLEMENTED(- (instancetype) init)

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
