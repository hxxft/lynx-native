// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef DOCUMENT_OBJECT_H_
#define DOCUMENT_OBJECT_H_

#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>

#include "body_object.h"
#include "thread_manager.h"
#include "base/weak_ptr.h"

namespace jscore {
    class JSCoreRuntime;
}

@protocol DocumentObjectProtocol <JSExport>
@property(nonatomic, strong, readwrite) NSString* cookie;
@property(nonatomic, strong, readwrite) BodyObject* body;
@property(nonatomic, strong, readwrite) NSString* domain;
-(id) createDom:(id)adapter;
-(id) createElement:(NSString*)tag;
-(id) addEvent:(NSString*)event Listener:(JSValue*)callback;
-(id) removeEvent:(NSString*)event Listener:(JSValue*)callback;
-(id) dispatchEvent:(NSString*)event;
@end

@interface DocumentObject : NSObject <DocumentObjectProtocol> {
@public
    base::WeakPtr<jscore::JSCoreRuntime> runtime_;
    BodyObject* body_object_;
    NSString* domain_;
}
-(id)initWithRuntime:(base::WeakPtr<jscore::JSCoreRuntime>)runtime;
@end

#endif /* DOCUMENT_OBJECT_H_ */
