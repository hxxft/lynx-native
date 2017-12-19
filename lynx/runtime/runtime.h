// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_RUNTIME_H_
#define LYNX_RUNTIME_RUNTIME_H_

#include <string>
#include <loader/html/html_loader.h>

#include "loader/loader.h"
#include "base/scoped_ptr.h"
#include "base/string/platform_string.h"
#include "render/render_object.h"
#include "render/body.h"
#include "runtime/thread_manager.h"
#include "net/url_request_context.h"
#include "runtime/js_vm.h"
#include "render/render_tree_host.h"

#include "inspector/inspector.h"

namespace lynx {
    class RenderTreeHost;
}

namespace jscore {
class JSContext;
class Runtime {
 public:
    Runtime(JSContext* context);
    ~Runtime() {}
    void InitRuntime(const char* arg);
    void RunScript(const base::PlatformString& source);
    void LoadScript(const std::string& source);
    void FlushScript();
    void LoadUrl(const std::string& url);
    void LoadUrl(const std::string& url, int type);
    void LoadScript(const std::string& url, int type);
    void LoadHTML(const std::string& url);
    void LoadHTML(const std::string& url, const std::string& html);
    void Reload(bool force);
    void Destroy();
    void AddJavaScriptInterface(const std::string& name,
                                LynxFunctionObject* object);

    std::string GetUserAgent();
    std::string GetPageUrl();

    lynx::RenderTreeHost* SetupRenderHost();

    lynx::RenderTreeHost* render_tree_host() {
        return render_tree_host_.Get();
    }

    ThreadManager* thread_manager() {
        return thread_manager_.Get();
    }

    net::URLRequestContext* url_requset_context() {
        return url_request_context_.Get();
    }

private:
    void InitRuntimeOnJSThread(const char* arg);
    void RunScriptOnJSThread(const base::PlatformString& source);
    void LoadScriptOnJSThread(const std::string& source);
    void LoadUrlOnJSThread(const std::string& url);
    void ReloadOnJSThread(bool force);
    void DestroyOnJSThread();
    void AddJavaScriptInterfaceOnJSThread(const std::string& name,
                                          base::ScopedPtr<LynxFunctionObject> object);
 protected:
    base::ScopedPtr<ThreadManager> thread_manager_;
    base::ScopedRefPtr<lynx::RenderTreeHost> render_tree_host_;
 private:
    base::ScopedPtr<net::URLRequestContext> url_request_context_;
    base::ScopedRefPtr<loader::LynxLoader> loader_;
    base::ScopedPtr<JSContext> context_;
    base::ScopedRefPtr<JSVM> vm_;
    base::WeakPtr<Runtime> weak_ptr_;
    
    base::ScopedRefPtr<debug::Inspector> inspector_;
};

}  // namespace jscore

#endif  // LYNX_RUNTIME_RUNTIME_H_
