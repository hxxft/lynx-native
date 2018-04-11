// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_BASE_FILE_PATH_UTILITY_H_
#define UI_BASE_FILE_PATH_UTILITY_H_

#import <Foundation/Foundation.h>

@interface LynxFilePathUtility : NSObject


+ (NSString*)toFilePath:(NSString*) fileUrl;
+ (BOOL)isFilePath:(NSString*) url;

@end

#endif  // UI_BASE_FILE_PATH_UTILITY_H_
