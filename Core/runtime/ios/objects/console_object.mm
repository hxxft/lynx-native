// Copyright 2017 The Lynx Authors. All rights reserved.

#include "console_object.h"

@implementation ConsoleObject

-(id) time
{
    return @([[NSDate date] timeIntervalSince1970] * 1000);
}

-(id) log:(id) value
{
    NSLog(@"js log: %@", value);
    return nil;
}

-(id) warn:(id) value
{
    NSLog(@"js warn: %@", value);
    return nil;
}

-(id) alert:(id) value
{
    NSLog(@"js alert: %@", value);
    return nil;
}

-(id) error:(id) value
{
    NSLog(@"js error: %@", value);
    return nil;
}
@end
