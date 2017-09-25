// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

typedef void(^cbImageDownloadFinish)(UIImage* image);

@interface ImageDownLoader : NSObject

+(instancetype) shareInstance;
-(BOOL) downloadImage:(NSString*)url then:(cbImageDownloadFinish)block;

@end
