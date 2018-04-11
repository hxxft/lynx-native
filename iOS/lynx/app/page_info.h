// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_APP_PAGE_INFO_H_
#define UI_APP_PAGE_INFO_H_

#import <Foundation/Foundation.h>

@interface LynxPageInfo : NSObject

@property (readonly, nonatomic) NSString* name;

- (id) initWithDict:(NSDictionary*)dict;

@end

#endif  // UI_APP_PAGE_INFO_H_
