//
//  LynxFilePathUtility.m
//  lynx
//
//  Created by dli on 2017/9/25.
//  Copyright © 2017年 lynx. All rights reserved.
//

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

