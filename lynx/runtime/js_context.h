// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_JS_CONTEXT_H_
#define LYNX_RUNTIME_JS_CONTEXT_H_

#include "base/observer/observer_list.h"
#include "base/ref_counted_ptr.h"
#include "runtime/js_vm.h"
#include "user_agent.h"

namespace jscore {
class Runtime;
class History;
class Location;
class Navigator;
class LynxFunctionObject;
class JSContext {
 public:
    JSContext()
            : runtime_(NULL),
              vm_(NULL),
              ua_() {

    }

    virtual ~JSContext() {
        Finalize();
    }

    virtual void Initialize(JSVM* vm, Runtime* runtime) {
        vm_ = vm;
        runtime_ = runtime;
    }
    virtual std::string RunScript(const char* source) {
        return "";
    }
    
    virtual void LoadUrl(const std::string& url) {}

    virtual void AddJavaScriptInterface(const std::string &name,
                                        LynxFunctionObject *object) {}

    void* GetVM() {
        return vm_->vm();
    }

    Runtime* runtime() {
        return runtime_;
    }

    void AddObserver(base::Observer* obs) {
        observers_.AddObserver(obs);
    }

    void RemoveObserver(base::Observer* obs) {
        observers_.RemoveObserver(obs);
    }

    void Finalize() {
        observers_.ForEachObserver();
    }

    void SetUserAgent(const std::string& ua) {
        ua_.Set(ua);
    }

    const std::string GetUserAgent() {
        return ua_.Get();
    }

    std::string GetPageUrl();

 protected:
    Runtime* runtime_;
    JSVM* vm_;
    base::ObserverList observers_;
    Location* location_;
    UserAgent ua_;
};
}

#endif  // LYNX_RUNTIME_JS_CONTEXT_H_
