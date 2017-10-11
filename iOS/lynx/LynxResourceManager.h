// Copyright 2017 The Lynx Authors. All rights reserved.



#import <Foundation/Foundation.h>
#import "LynxResourceReader.h"

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

