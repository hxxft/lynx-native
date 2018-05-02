// Copyright 2017 The Lynx Authors. All rights reserved.

#import "jscore_runtime.h"

#include "callback.h"
#include "common.h"
#include "runtime_url_request_delegate.h"
#include "url_request_context.h"
#include "loader/xc/xc_file.h"
#include "loader/xc/xc_loader.h"
#import "timeout_callback.h"

using namespace std;

namespace jscore {

    JSCoreRuntime::JSCoreRuntime() : weak_runtime_(this){
        loader_ = new loader::XCLoader(this);
    }

    void JSCoreRuntime::InitRuntime(const char* arg) {
        thread_manager_->RunOnJSThread(base::Bind(&JSCoreRuntime::InitRuntimeOnJSThread, weak_runtime_));
    }

    void JSCoreRuntime::LoadScript(const std::string &source) {
        base::PlatformString str = base::PlatformString([[NSString alloc]initWithUTF8String:source.c_str()]);
        RunScriptOnJSThread(str);
    }
    
    void JSCoreRuntime::RunScript(const base::PlatformString& source) {
        thread_manager_->RunOnJSThread(base::Bind(&JSCoreRuntime::RunScriptOnJSThread, weak_runtime_, source));
    }

    void JSCoreRuntime::LoadUrl(const string& url) {
        thread_manager_->RunOnJSThread(base::Bind(&JSCoreRuntime::LoadUrlOnJSThread, weak_runtime_, url));
    }

    void JSCoreRuntime::LoadUrlOnJSThread(const string& url) {
        [location_object_ setUrl:[NSString stringWithCString:url.c_str() encoding:NSUTF8StringEncoding]];
        [history_object_ goToUrl:[NSString stringWithUTF8String:url.c_str()]];
        loader_->Load(url, loader::XCFile::XC_URL);
    }

    void JSCoreRuntime::Reload(bool force) {
        thread_manager_->RunOnJSThread(base::Bind(&JSCoreRuntime::ReloadOnJSThread, weak_runtime_, force));
    }

    void JSCoreRuntime::ReloadOnJSThread(bool force) {
        std::string url([location_object_.href UTF8String]);
        [history_object_ reloadUrl:location_object_.href];
        LoadUrlOnJSThread(url);
    }

    void JSCoreRuntime::InitRuntimeOnJSThread() {
        context_ = [[JSContext alloc] init];

        context_.exceptionHandler = ^(JSContext* context, JSValue* exception)
        {
            context.exception = exception;
            NSString* error = [NSString stringWithFormat:@"%@", exception];
            // type of String
            NSString* stacktrace = [NSString stringWithFormat:@"%@", [exception objectForKeyedSubscript:@"stack"]];
            // type of Number
            NSString* lineNumber = [NSString stringWithFormat:@"%@", [exception objectForKeyedSubscript:@"line"]];
            // type of Number
            NSString* column = [NSString stringWithFormat:@"%@", [exception objectForKeyedSubscript:@"column"]];
            NSLog(@"js Error: %@, Stack: %@ , Line number in file: %@, column: %@", error, stacktrace, lineNumber, column);
        };

        screen_object_ = [[ScreenObject alloc] init];
        loader_object_ = [[LoaderObject alloc] initWithRuntime:weak_runtime_];
        window_object_ = [[WindowObject alloc] initWithContext:context_];
        location_object_ = [[LocationObject alloc] initWithRuntime:weak_runtime_];
        history_object_ = [[HistoryObject alloc] initWithRuntime:weak_runtime_];
        console_object_ = [[ConsoleObject alloc] init];
        navigator_object_ = [[NavigatorObject alloc] init];
        document_object_ = [[DocumentObject alloc] initWithRuntime:weak_runtime_];
        
        TimeoutCallback *timer_object = [[TimeoutCallback alloc] initWithContext:context_ WithRuntime:weak_runtime_];
        
        context_[@"document"] = document_object_;
        context_[@"location"] = location_object_;
        context_[@"console"] = console_object_;
        context_[@"navigator"] = navigator_object_;
        context_[@"window"] = context_.globalObject;
        context_[@"global"] = context_.globalObject;
        context_[@"screen"] = screen_object_;
        context_[@"loader"] = loader_object_;
        context_[@"history"] = history_object_;
    }

    void JSCoreRuntime::RunScriptOnJSThread(const base::PlatformString& source) {
        NSString *script_src = source.GetString();
        if (script_src) {
            [context_ evaluateScript:script_src];
        }
    }

}
