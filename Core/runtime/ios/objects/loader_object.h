// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef loader_object_h
#define loader_object_h

#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>
#import <UIKit/UIKit.h>
#include "url_request_delegate.h"
#include "base/weak_ptr.h"
#include "js_function_wrap.h"


namespace jscore {
    class JSCoreRuntime;
    class LoaderRequestDelegate : public net::URLRequestDelegate {
    
    public:
        LoaderRequestDelegate(base::WeakPtr<JSCoreRuntime>& runtime) : weak_runtime_(runtime), weak_ptr_(this){}
        LoaderRequestDelegate(base::WeakPtr<JSCoreRuntime>& runtime, JSFunctionWrap* succ)
            : weak_runtime_(runtime), js_succ_function_(succ), weak_ptr_(this) {}
        LoaderRequestDelegate(base::WeakPtr<JSCoreRuntime>& runtime, JSFunctionWrap* succ, JSFunctionWrap* error)
            : weak_runtime_(runtime), js_succ_function_(succ), js_error_function_(error), weak_ptr_(this) {}
        virtual ~LoaderRequestDelegate() {}
        virtual void OnSuccess(const base::PlatformString& url, const base::PlatformString& response);
        virtual void OnFailed(const base::PlatformString& url, const base::PlatformString& error);
        void OnSuccessOnJSThread();
        void OnFailedOnJSThread();
    
    private:
        base::WeakPtr<JSCoreRuntime> weak_runtime_;
        base::ScopedPtr<JSFunctionWrap> js_succ_function_;
        base::ScopedPtr<JSFunctionWrap> js_error_function_;
        base::WeakPtr<LoaderRequestDelegate> weak_ptr_;
    };
}

@protocol LoaderObjectProtocol <JSExport>

-(void) trace:(NSString*)url;
JSExportAs(script, -(void)script:(NSString*)url onSuccess:(JSValue*)sc onError:(JSValue*)ec);

@end


@interface LoaderObject : NSObject <LoaderObjectProtocol>
{
    @private
    base::WeakPtr<jscore::JSCoreRuntime> runtime_;
}

-(id) initWithRuntime: (base::WeakPtr<jscore::JSCoreRuntime>) runtime;
-(void)script:(NSString*)url onSuccess:(JSValue*)sc onError:(JSValue*)ec;

@end

#endif /* loader_object_h */
