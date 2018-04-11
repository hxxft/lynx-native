// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_APP_RESOURCE_MANAGER_H_
#define UI_APP_RESOURCE_MANAGER_H_

#import <Foundation/Foundation.h>

#include "app/resource_reader.h"

@interface LynxResourceManager : NSObject {
    @private
    NSString* asset_mode_application_location_;
    NSString* data_mode_application_location_;
    NSString* debug_mode_application_location_;
    
    NSString* asset_protocol_;
    NSString* data_protocol_;
    NSString* debug_protocol_;
}

- (id) init;
+ (LynxResourceManager*) instance;

@property (readonly, nonatomic) LynxResourceReader* reader;

- (NSURL*) toResourceURL:(NSString*) resource;

@end

#endif  // UI_APP_RESOURCE_MANAGER_H_

