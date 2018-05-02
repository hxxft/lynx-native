// Copyright 2017 The Lynx Authors. All rights reserved.

#include "debugger/ios/debug_host_impl.h"

#include "lynx_view_controller.h"

#import <UIKit/UIKit.h>

@implementation DebugHostImplBridge

-(void) DispatchReload {
   
        //Update UI in UI thread here
        
    UINavigationController *controller = [UIApplication sharedApplication].keyWindow.rootViewController;
    [controller popViewControllerAnimated:false];
    [controller pushViewController:[[LynxViewController alloc] initWithProtocal:@"Debug://" forName:@""] animated:false];
    
}

-(void) runWithDebugWithType:(debug::DebugType) type {
    switch (type) {
        case debug::Debug_Reload:
            [self DispatchReload];
            break;
        default:
            break;
    }
    
}

@end

namespace debug {
    
    DebugHostImpl::DebugHostImpl() {
        bridge_ = [DebugHostImplBridge alloc];
    }
    
    DebugHostImpl::~DebugHostImpl() {
        bridge_ = nil;
    }
    
    void DebugHostImpl::Run(DebugType type) {
        [bridge_ runWithDebugWithType:type];
    }
}
