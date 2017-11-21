// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LynxResourceManager.h"

static LynxResourceManager* g_resource_manager_ = nil;

@implementation LynxResourceManager 

+ (LynxResourceManager*) instance {
    if(g_resource_manager_ == nil) {
        g_resource_manager_ = [[LynxResourceManager alloc] init];
    }
    return g_resource_manager_;
}

- (id) init {
    self = [super init];
    if (self) {
        asset_mode_application_location_ = [NSString stringWithFormat:@"file://%@", [[NSBundle mainBundle] pathForResource:@"assets" ofType:@"bundle"]];
        data_mode_application_location_ = @"file://";
        debug_mode_application_location_ = @"http://127.0.0.1:3000/";
        
        asset_protocol_ = @"Asset://";
        data_protocol_ = @"Data://";
        debug_protocol_ = @"Debug://";
        
        _reader = [LynxResourceReader alloc];
    }
    return self;
}

- (NSURL*) toResourceURL:(NSString*) resource {
    NSURL* url = nil;
    NSRange range;
    if((range = [resource rangeOfString:asset_protocol_]).location != NSNotFound) {
        url = [NSURL URLWithString:[NSString stringWithFormat:@"%@/%@", asset_mode_application_location_, [resource substringFromIndex:range.length]]];
    }else if((range = [resource rangeOfString:data_protocol_]).location != NSNotFound) {
        url = [NSURL URLWithString:[NSString stringWithFormat:@"%@/%@", data_mode_application_location_, [resource substringFromIndex:range.length]]];
    }else if((range = [resource rangeOfString:debug_protocol_]).location != NSNotFound){
        url = [NSURL URLWithString:[NSString stringWithFormat:@"%@/%@", debug_mode_application_location_, [resource substringFromIndex:range.length]]];
    }
    return url;
}

@end
