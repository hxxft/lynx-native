// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef JSCOR_RUNTIME_H_
#define JSCOR_RUNTIME_H_

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>

#include <string>

#include "runtime.h"
#include "thread_manager.h"

#import "console_object.h"
#import "document_object.h"
#import "js_function_wrap.h"
#import "navigator_object.h"
#import "location_object.h"
#import "window_object.h"
#import "screen_object.h"
#import "loader_object.h"
#import "history_object.h"

namespace jscore {

    class JSCoreRuntime : public Runtime {
    public:
        JSCoreRuntime();
        virtual ~JSCoreRuntime() {weak_runtime_.Invalidate(); context_ = nil;}
        virtual void InitRuntime(const char* arg);
        virtual void LoadUrl(const std::string& url);
        virtual void Reload(bool force);
        virtual void RunScript(const base::PlatformString& source);
        virtual void LoadScript(const std::string& source);
    private:
        void InitRuntimeOnJSThread();
        void RunScriptOnJSThread(const base::PlatformString& source);
        void LoadUrlOnJSThread(const std::string& url);
        void ReloadOnJSThread(bool force);
        JSContext* context_;
        base::WeakPtr<JSCoreRuntime> weak_runtime_;
        ScreenObject* screen_object_;
        LoaderObject* loader_object_;
        WindowObject* window_object_;
        LocationObject* location_object_;
        ConsoleObject* console_object_;
        NavigatorObject* navigator_object_;
        DocumentObject* document_object_;
        HistoryObject* history_object_;
    };
}

#endif /* JSCOR_RUNTIME_H_ */
