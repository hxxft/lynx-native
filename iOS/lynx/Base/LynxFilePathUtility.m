// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LynxFilePathUtility.h"

@implementation LynxFilePathUtility

static NSString* ASSETS_PROTOCOL = @"Assets://";

+ (BOOL)isFilePath:(NSString*) url {
    return [url hasPrefix:ASSETS_PROTOCOL];
}

+ (NSString*)toFilePath:(NSString*) fileUrl {
    NSString* fileType = [fileUrl pathExtension];
    NSString* filePath = [[fileUrl stringByDeletingPathExtension]substringFromIndex:ASSETS_PROTOCOL.length-1];
    return [[NSBundle mainBundle] pathForResource:filePath ofType:fileType];
}

@end

