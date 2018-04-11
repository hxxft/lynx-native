// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_WIDGET_IMAGE_IMAGE_DOWNLOADER_H_
#define UI_WIDGET_IMAGE_IMAGE_DOWNLOADER_H_

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

typedef void(^cbImageDownloadFinish)(UIImage* image);

@interface ImageDownLoader : NSObject

+(instancetype) shareInstance;
-(BOOL) downloadImage:(NSString*)url then:(cbImageDownloadFinish)block;

@end

#endif  // UI_WIDGET_IMAGE_IMAGE_DOWNLOADER_H_
