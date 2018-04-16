// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/file_path_utility.h"

@implementation LynxFilePathUtility

static NSString* ASSETS_PROTOCOL = @"Assets://";

+ (BOOL)isFilePath:(NSString*) url {
    return [url hasPrefix:ASSETS_PROTOCOL];
}

+ (NSString*)toFilePath:(NSString*) fileUrl {
    
    NSString* fileType = [fileUrl pathExtension];
    
    NSString* filePath = [[fileUrl stringByDeletingPathExtension]substringFromIndex:ASSETS_PROTOCOL.length-1];
    
    NSBundle * bundle = [NSBundle mainBundle];
    
    if ([filePath containsString:@".bundle"]) {
        
        NSArray<NSString *> * pathArray =  [filePath componentsSeparatedByString:@".bundle"];
        
        NSString * bundlePath = pathArray.firstObject;
        
        bundle =  [NSBundle bundleWithPath:[[NSBundle mainBundle] pathForResource:bundlePath ofType:@"bundle"]];
        
        filePath =  (pathArray.count > 1) ? pathArray[1] : filePath;
    }
    
    return [bundle pathForResource:filePath ofType:fileType];
}

@end

