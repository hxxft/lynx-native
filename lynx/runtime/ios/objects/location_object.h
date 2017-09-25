// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>
#include "uri.h"
#include "base/weak_ptr.h"

namespace jscore {
    class JSCoreRuntime;
}

@protocol LocationObjectProtocol<JSExport>

@property(nonatomic, strong, readwrite) NSString* href;
@property(nonatomic, strong, readwrite) NSString* hash;
@property(nonatomic, strong, readwrite) NSString* protocol;
@property(nonatomic, strong, readwrite) NSString* pathname;
@property(nonatomic, strong, readwrite) NSString* origin;
@property(nonatomic, strong, readwrite) NSString* host;
@property(nonatomic, strong, readwrite) NSString* hostname;
@property(nonatomic, strong, readwrite) NSNumber* port;
@property(nonatomic, strong, readwrite) NSString* search;

// Replace current document with new one, this method does not generate a new record in the history object.
// When using this method, the new URL will overwrite the current record in the history object.
-(void) replace:(NSString *) href;

// While force is false, check if the content has changed according to If-Modified-Since of HTTP header, if changed, reload content, if not, nothing to do.
// While force is true, reload content.
-(void) reload:(bool) force;

// This method uses to load a new document.
-(void) assign:(NSString *) url;

@end

@interface LocationObject : NSObject<LocationObjectProtocol>
{
    base::WeakPtr<jscore::JSCoreRuntime> runtime_;
    net::Uri uri_;
}

-(id) initWithRuntime:(base::WeakPtr<jscore::JSCoreRuntime>)runtime;
-(void) setUrl:(NSString *) href;
-(void) parseHref:(NSString *) href;
@end
