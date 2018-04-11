// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_APP_RESOURCE_READER_H_
#define UI_APP_RESOURCE_READER_H_

#import <Foundation/Foundation.h>

@interface LynxResourceReader : NSObject

- (NSString*) readResourceAsString: (NSURL*)url;

- (NSDictionary*) readResourceAsJSON: (NSURL*)url;

@end

#endif  // UI_APP_RESOURCE_READER_H_
