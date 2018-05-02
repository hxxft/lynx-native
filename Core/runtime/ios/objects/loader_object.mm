// Copyright 2017 The Lynx Authors. All rights reserved.

#import "loader_object.h"
#import "url_request.h"
#import "url_request_context.h"
#include "jscore_runtime.h"

namespace jscore {

    
    void LoaderRequestDelegate::OnSuccess(base::PlatformString& url, base::PlatformString& response) {
        if (weak_runtime_.IsValid()) {
            weak_runtime_.Get()->RunScript(response);
            weak_runtime_.Get()->thread_manager()->RunOnJSThread(base::Bind(&LoaderRequestDelegate::OnSuccessOnJSThread, weak_ptr_));
        }
    }
    
    void LoaderRequestDelegate::OnFailed(base::PlatformString& url, base::PlatformString& error) {
        
        if (weak_runtime_.IsValid()) {
            weak_runtime_.Get()->thread_manager()->RunOnJSThread(base::Bind(&LoaderRequestDelegate::OnFailedOnJSThread, weak_ptr_));
        }
    }
    
    void LoaderRequestDelegate::OnSuccessOnJSThread() {
        if (js_succ_function_.Get() != NULL) {
            js_succ_function_->Run();
        }
        Release();
    }
    
    void LoaderRequestDelegate::OnFailedOnJSThread() {
        if (js_error_function_.Get() != NULL) {
            js_error_function_->Run();
        }
        Release();
    }

}

@implementation LoaderObject
{
    
}

-(id) initWithRuntime:(base::WeakPtr<jscore::JSCoreRuntime>)runtime
{
    self = [super init];
    if (self) {
        runtime_ = runtime;
    }
    return self;
}

-(void) trace:(NSString*)url
{
}

-(void)script:(NSString*)url onSuccess:(JSValue*)succCallback onError:(JSValue*)errorCallback {
    jscore::JSFunctionWrap* succ = succCallback ? new jscore::JSFunctionWrap(succCallback) : NULL;
    jscore::JSFunctionWrap* error = errorCallback ? new jscore::JSFunctionWrap(errorCallback) : NULL;
    jscore::LoaderRequestDelegate* delegate = new jscore::LoaderRequestDelegate(runtime_, succ, error);
    delegate->AddRef();
    base::ScopedPtr<net::URLRequest> request(net::URLRequestContext::CreateRequest());
    request->Fetch([url UTF8String], delegate);
}

@end
