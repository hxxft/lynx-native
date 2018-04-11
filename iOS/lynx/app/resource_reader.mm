// Copyright 2017 The Lynx Authors. All rights reserved.

#include "app/resource_reader.h"

@implementation LynxResourceReader

- (NSString*) readResourceAsString: (NSURL*)url {
    NSString* content = nil;
    NSData* fileData = [NSData dataWithContentsOfURL:url];
    if (nil != fileData && fileData.length > 0)
    {
        content = [[NSString alloc] initWithData:fileData
                                                     encoding:NSUTF8StringEncoding];
    }
    return content;
}

- (NSDictionary*) readResourceAsJSON: (NSURL*)url {
    NSDictionary *dict = nil;
    NSData* fileData = [NSData dataWithContentsOfURL:url];
    if (nil != fileData && fileData.length > 0)
    {
        dict = [NSJSONSerialization JSONObjectWithData:fileData options:NSJSONReadingAllowFragments error:nil];
    }
    return dict;
    return nil;
}

@end
