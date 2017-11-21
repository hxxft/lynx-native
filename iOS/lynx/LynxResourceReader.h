// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>

@interface LynxResourceReader : NSObject

- (NSString*) readResourceAsString: (NSURL*)url;

- (NSDictionary*) readResourceAsJSON: (NSURL*)url;

@end
