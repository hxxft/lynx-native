// Copyright 2017 The Lynx Authors. All rights reserved.

#import "navigator_object.h"
#import "../../config.h"

@implementation NavigatorObject

@synthesize userAgent;
@synthesize appName;
@synthesize appCodeName;
@synthesize appVersion;
@synthesize platform;

- (NSString *)userAgent {
    return [NSString stringWithCString:USERAGENT.c_str() encoding:[NSString defaultCStringEncoding]];
}

- (NSString *)appName {
    return [NSString stringWithCString:APP_NAME encoding:[NSString defaultCStringEncoding]];
}

- (NSString *)appCodeName {
    return [NSString stringWithCString:APP_NAME encoding:[NSString defaultCStringEncoding]];
}

- (NSString *)appVersion {
    return [NSString stringWithCString:VERSION encoding:[NSString defaultCStringEncoding]];
}

- (NSString *)platform {
    return [NSString stringWithCString:PLATFORM encoding:[NSString defaultCStringEncoding]];
}
@end
