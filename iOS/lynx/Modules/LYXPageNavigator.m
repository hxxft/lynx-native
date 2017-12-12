// Copyright 2017 The Lynx Authors. All rights reserved.
#import "LYXPageNavigator.h"

@implementation LYXPageNavigator

LYX_REGISTER_MODULE(PageNavigator)

LYX_REGISTER_METHOD(void, startNewPage:(NSString *) page) {
    dispatch_sync(dispatch_get_main_queue(), ^{
        //Update UI in UI thread here
        
    });
}


@end
