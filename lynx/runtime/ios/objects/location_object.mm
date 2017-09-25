// Copyright 2017 The Lynx Authors. All rights reserved.

#import "location_object.h"
#include "jscore_runtime.h"

@implementation LocationObject

@synthesize port;
@synthesize host;
@synthesize hostname;
@synthesize href;
@synthesize pathname;
@synthesize origin;
@synthesize search;
@synthesize protocol;

-(NSString *) href
{
    return self->href;
}

-(void) replace:(NSString*) href
{
    [self parseHref: href];
    if (runtime_.IsValid()) {
        runtime_.Get()->Reload(true);
    }
}

-(void) reload:(bool)force
{
    if (runtime_.IsValid()) {
        runtime_.Get()->Reload(force);
    }
    
}

-(void) assign:(NSString *)url
{
    if (runtime_.IsValid()) {
        runtime_.Get()->LoadUrl(std::string([url UTF8String]));
    }
    
}

-(void) setUrl:(NSString *)href
{
    [self parseHref:href];
}

-(void) parseHref:(NSString *) href
{
    uri_ = net::Uri::Parse(std::string([href UTF8String]));
    self.href = [NSString stringWithString:href];
    self.hostname = [NSString stringWithUTF8String:uri_.hostname_.c_str()];
    self.host = [NSString stringWithUTF8String:uri_.host_.c_str()];
    NSString *port_str = [NSString stringWithUTF8String:uri_.port_.c_str()];
    self.port = @([port_str integerValue]);
    self.search = [NSString stringWithUTF8String:uri_.search_.c_str()];
    self.protocol = [NSString stringWithUTF8String:uri_.protocol_.c_str()];
    self.pathname = [NSString stringWithUTF8String:uri_.pathname_.c_str()];
    self.origin = [NSString stringWithUTF8String:uri_.origin_.c_str()];
}

-(id) initWithRuntime:(base::WeakPtr<jscore::JSCoreRuntime>)runtime
{
    self = [super init];
    if(self != nil) {
        self->runtime_ = runtime;
    }
    return self;
}

@end
