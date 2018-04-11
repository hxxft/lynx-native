// Copyright 2017 The Lynx Authors. All rights reserved.

#include "widget/image/image_downloader.h"
#include "base/file_path_utility.h"

#import "UIImageView+WebCache.h"

@implementation ImageDownLoader

+(instancetype) shareInstance
{
    static dispatch_once_t _onceToken;
    static ImageDownLoader* sharedInstance = nil;
    
    dispatch_once(&_onceToken, ^{
        sharedInstance = [[self alloc] init];
    });
    return sharedInstance;
}

-(BOOL) downloadImage:(NSString*)url then:(cbImageDownloadFinish)block {
    SDWebImageManager *manager = [SDWebImageManager sharedManager];
    
    NSString* path = [LynxFilePathUtility toFilePath:url];
    if(path == nil)
        return false;
    NSURL* ns_url = [NSURL fileURLWithPath:path];
    [manager loadImageWithURL:ns_url
                          options:SDWebImageRetryFailed
                         progress:^(NSInteger receivedSize, NSInteger expectedSize, NSURL *targetURL) {}
                        completed:^(UIImage *image, NSData *data, NSError *error, SDImageCacheType cacheType, BOOL finished, NSURL *imageURL)     {
         block(image);
     }];
    return YES;
}

@end
