// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_JS_CONTEXT_H_
#define LYNX_RUNTIME_JS_CONTEXT_H_

#include "base/ref_counted_ptr.h"
#include "base/observer/observer_list.h"
#include "runtime/user_agent.h"
#include "runtime/js/js_vm.h"
#include "runtime/js/class_template_storage.h"

namespace jscore {
class Runtime;
class Global;
class LynxObjectPlatform;
class JSContext : public base::RefCountPtr<Runtime> {
 public:
    JSContext()
            : runtime_(NULL),
              vm_(NULL),
              ua_(),
              class_template_storage_(lynx_new ClassTemplateStorage),
              global_(NULL) {

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
    
    virtual void LoadUrl(const std::string& url) = 0;

    virtual void AddJavaScriptInterface(const std::string &name,
                                        base::ScopedPtr<LynxObjectPlatform> object) = 0;
    template<typename T = void*>
    T GetVM() {
        return static_cast<T>(vm_->vm());
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

    void OnExceptionOccurred(std::string &error);

    inline ClassTemplateStorage* class_template_storage() {
        return class_template_storage_.Get();
    }

    inline Global* global() {
        return global_;
    }

 protected:
    Runtime* runtime_;
    JSVM* vm_;
    base::ObserverList observers_;
    UserAgent ua_;
    base::ScopedPtr<ClassTemplateStorage> class_template_storage_;
    Global* global_;

    DISALLOW_COPY_AND_ASSIGN(JSContext);
};
}

#endif  // LYNX_RUNTIME_JS_CONTEXT_H_
