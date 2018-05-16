// Copyright 2017 The Lynx Authors. All rights reserved.
#include "modules/page_navigator.h"

#include "lynx_view_controller.h"
#import <UIKit/UIKit.h>

@implementation LxPageNavigator

LX_REGISTER_MODULE(PageNavigator)

LX_REGISTER_METHOD(void, startNewPage:(NSString *) page) {
    dispatch_sync(dispatch_get_main_queue(), ^{
        //Update UI in UI thread here
        
        UINavigationController *controller = (UINavigationController *)[UIApplication sharedApplication].keyWindow.rootViewController;

        [controller pushViewController:[[LynxViewController alloc] initWithName:page] animated:false];
        
    });
}


@end
