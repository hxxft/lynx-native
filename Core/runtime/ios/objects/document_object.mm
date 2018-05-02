// Copyright 2017 The Lynx Authors. All rights reserved.

#include "common.h"
#include "document_object.h"
#include "element_object.h"
#include "jscore_runtime.h"

using namespace jscore;

@implementation DocumentObject

@synthesize body, cookie, domain;

-(id) createDom:(id)adapter
{
    return nil;
}

-(id) createElement:(NSString*)tag
{
    if (runtime_.IsValid()) {
        ElementObject* object = [[ElementObject alloc] initWithTagName:tag
                                                           threadProxy:runtime_.Get()->thread_manager()
                                                                  host:runtime_.Get()->render_tree_host()];
        return object;
    }
    return nil;
}

- (id)initWithRuntime:(base::WeakPtr<jscore::JSCoreRuntime>)runtime {
    self = [super init];
    if(self != nil) {
        self->runtime_ = runtime;
        if (runtime_.IsValid()) {
            self->body_object_ = [[BodyObject alloc] initWithHost:runtime_.Get()->render_tree_host()];
        }
    }
    return self;
}

-(BodyObject*) body
{
    return body_object_;
}

// note:
-(NSString*) cookie
{
    return @"";
}

-(id) addEvent:(NSString*)event Listener:(JSValue*)value
{
    return nil;
}

-(id) removeEvent:(NSString *)event Listener:(JSValue *)callback {
    return nil;
}

-(id) dispatchEvent:(NSString *)event {
    return nil;
}

- (id) domain {
    return domain_;
}

- (void) domain: (NSString *)domainstr {
    domain_ = domainstr;
}


@end
