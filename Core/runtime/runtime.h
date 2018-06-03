// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_RUNTIME_H_
#define LYNX_RUNTIME_RUNTIME_H_

#include <string>
#include "loader/html/html_loader.h"

#include "loader/loader.h"
#include "base/scoped_ptr.h"
#include "base/string/platform_string.h"
#include "render/render_object.h"
#include "render/body.h"
#include "runtime/thread_manager.h"
#include "net/url_request_context.h"
#include "runtime/js/js_vm.h"
#include "render/render_tree_host.h"

//#include "inspector/inspector.h"
#include "debugger/debug_client.h"

namespace jscore {

class JSContext;
class LynxObjectPlatform;

class ResultCallback {
public:
    virtual void OnReceiveResult(const std::string &result) const {}
};

class Runtime : public base::RefCountPtr<Runtime>{
 public:
    Runtime(JSContext* context);
    ~Runtime();
    void InitRuntime(const char* arg);
    void RunScript(base::ScopedPtr<base::PlatformString> source,
                   base::ScopedPtr<ResultCallback> callback = base::ScopedPtr<ResultCallback>());
    void LoadScript(const std::string& source);
    void LoadScriptDataWithBaseUrl(const std::string& data, const std::string& url);
    void FlushScript();
    void LoadUrl(const std::string& url);
    void LoadUrl(const std::string& url, int type);
    void LoadScript(const std::string& url, int type);
    void LoadHTML(const std::string& url);
    void LoadHTML(const std::string& url, const std::string& html);
    void Reload(bool force);
    void Destroy();
    void Pause();
    void Resume();
    void AddJavaScriptInterface(const std::string& name,
                                LynxObjectPlatform* object);

    void SetUserAgent(const std::string& ua);
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

    inline void set_exception_handler(ResultCallback* handler) {
        exception_handler_.Reset(handler);
    }

    inline ResultCallback* exception_handler() {
        return exception_handler_.Get();
    }

private:
    void InitRuntimeOnJSThread(const char* arg);
    void RunScriptOnJSThread(base::ScopedPtr<base::PlatformString> source,
                             base::ScopedPtr<ResultCallback> callback);
    void LoadScriptOnJSThread(const std::string& source);
    void LoadUrlOnJSThread(const std::string& url);
    void ReloadOnJSThread(bool force);
    void DestroyOnJSThread();

 protected:
    base::ScopedPtr<ThreadManager> thread_manager_;
    base::ScopedRefPtr<lynx::RenderTreeHost> render_tree_host_;
 private:
    base::ScopedPtr<net::URLRequestContext> url_request_context_;
    base::ScopedRefPtr<loader::LynxLoader> loader_;
    base::ScopedRefPtr<JSVM> vm_;
    base::ScopedRefPtr<JSContext> context_;
    base::ScopedPtr<ResultCallback> exception_handler_;
    //base::ScopedRefPtr<debug::Inspector> inspector_;
    DISALLOW_COPY_AND_ASSIGN(Runtime);
};

}  // namespace jscore

#endif  // LYNX_RUNTIME_RUNTIME_H_
