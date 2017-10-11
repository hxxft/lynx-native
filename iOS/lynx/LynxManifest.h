// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_APP_LYNX_MANIFEST_H_
#define LYNX_APP_LYNX_MANIFEST_H_

#import <Foundation/Foundation.h>
#import "LynxApplicationInfo.h"

@interface LynxManifest : NSObject {
    LynxApplicationInfo* application_info_;
}
@end

#endif  // LYNX_APP_LYNX_MANIFEST_H_
