// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/js/js_context.h"
#include "runtime/runtime.h"
#include "runtime/global.h"
#include "parser/render_parser.h"
#include "render/render_tree_host_impl.h"
#include "runtime/base/lynx_object_platform.h"

#include "base/trace_event/trace_event_common.h"
#include "base/log/logging.h"

#include "plugin/plugin_server.h"
#include "base/threading/completion_event.h"

namespace jscore {

    Runtime::Runtime(JSContext* context)
            : thread_manager_(lynx_new ThreadManager()),
              url_request_context_(lynx_new net::URLRequestContext(this)),
              loader_(lynx_new loader::HTMLLoader(this)),
              context_(context) {
#if ENABLE_TRACING
      base::TraceLogger::Instance()->Start();
#endif
      //inspector_ = new debug::Inspector(thread_manager_->js_thread(), "InspectorThread");
      AddRef();
    }
    
    Runtime::~Runtime() {
        DLOG(INFO) << "Destruct Runtime";
    }
    
    void Runtime::InitRuntime(const char* arg) {
        thread_manager_->RunOnJSThread(
                                       base::Bind(&Runtime::InitRuntimeOnJSThread, base::ScopedRefPtr<Runtime>(this), arg));
    }

    lynx::RenderTreeHost* Runtime::SetupRenderHost() {
        render_tree_host_ = lynx_new lynx::RenderTreeHost(context_.Get(),
                                                     thread_manager_.Get(), NULL);
        lynx::Body* root = lynx_new lynx::Body(thread_manager(), render_tree_host());
        render_tree_host_->SetRenderRoot(root);
        return render_tree_host_.Get();
    }

    void Runtime::LoadUrl(const std::string& url) {
        LoadUrl(url, loader::LynxLoader::MAIN_FILE);
    }

    void Runtime::LoadUrl(const std::string& url, int type) {
        if(type == loader::LynxLoader::MAIN_FILE) {
            render_tree_host_->set_page_location(url);
        }
        loader_->Load(url, type);
        thread_manager_->RunOnJSThread(
                base::Bind(&Runtime::LoadUrlOnJSThread, base::ScopedRefPtr<Runtime>(this), url));
    }

    void Runtime::LoadScript(const std::string& url, int type) {
        TRACE_EVENT0("js", "Runtime::LoadScript");
        std::string transformed_url = render_tree_host_->page_location() + url;
        loader_->Load(transformed_url, type);
    }

    void Runtime::Reload(bool force) {
        thread_manager_->RunOnJSThread(
                base::Bind(&Runtime::ReloadOnJSThread, base::ScopedRefPtr<Runtime>(this), force));
    }

    void Runtime::RunScript(base::ScopedPtr<base::PlatformString> source,
                            base::ScopedPtr<ResultCallback> callback) {
        TRACE_EVENT0("js", "Runtime::RunScript");
        thread_manager_->RunOnJSThread(
                base::Bind(&Runtime::RunScriptOnJSThread, base::ScopedRefPtr<Runtime>(this), source, callback));
    }

    void Runtime::LoadHTML(const std::string& url, const std::string& html) {
        render_tree_host_->set_page_location(url);
        LoadHTML(html);
    }

    void Runtime::LoadHTML(const std::string& html) {
        if (html.empty())
            return;
        render_tree_host()->host_impl()->SetParseStart();
        parser::RenderParser parser(render_tree_host(), this);
        parser.Insert(html);
        thread_manager_->RunOnJSThread(
            base::Bind(&lynx::RenderTreeHost::TreeSync, render_tree_host_)); 
        render_tree_host()->ForceFlushCommands();
        render_tree_host()->host_impl()->SetParseFinished();
    }

    void Runtime::LoadScript(const std::string& source) {
        thread_manager_->RunOnJSThread(
                base::Bind(&Runtime::LoadScriptOnJSThread, base::ScopedRefPtr<Runtime>(this), source));
    }

    void Runtime::LoadScriptDataWithBaseUrl(const std::string& data, const std::string& url) {
        render_tree_host_->set_page_location(url);
        thread_manager_->RunOnJSThread(
                base::Bind(&Runtime::LoadScriptOnJSThread, base::ScopedRefPtr<Runtime>(this), data));
    }

    void Runtime::FlushScript() {
        loader_->Flush();
    }

    void Runtime::AddJavaScriptInterface(const std::string& name, LynxObjectPlatform* object) {
        base::ScopedPtr<LynxObjectPlatform> scoped_ptr(object);
        thread_manager_->RunOnJSThread(
                base::Bind(&JSContext::AddJavaScriptInterface,
                           base::ScopedRefPtr<JSContext>(context_), name, scoped_ptr));
    }

    void Runtime::Destroy() {
        //inspector_->Detach();
        url_request_context_->Stop();
        thread_manager_->DetachUIThread();
        base::ScopedRefPtr<Runtime> ref(this);
        Release();
        thread_manager_->QuitJSThread(base::Bind(&Runtime::DestroyOnJSThread, ref));
#if ENABLE_TRACING
        base::TraceLogger::Instance()->Stop();
#endif
    }

    void Runtime::InitRuntimeOnJSThread(const char *arg) {
        TRACE_EVENT0("js", "Runtime::InitRuntimeOnJSThread");
        //vm_ = JSVM::Instance();
        vm_ = lynx_new JSVM;
        vm_->Initialize();
        context_->Initialize(vm_.Get(), this);
#if ENABLE_PLUGIN
        plugin::PluginServer::Register(context_->global()->plugin());
#endif
        //inspector_->Attach(this);
    }

    void Runtime::RunScriptOnJSThread(base::ScopedPtr<base::PlatformString> source,
                                      base::ScopedPtr<ResultCallback> callback) {
        TRACE_EVENT0("js", "Runtime::RunScriptOnJSThread");
        std::string result =
                context_->RunScript(source->GetUTFChars());
        if (callback.Get() != NULL) {
            callback->OnReceiveResult(result);
        }
    }

    void Runtime::LoadScriptOnJSThread(const std::string& source) {
        TRACE_EVENT0("js", "Runtime::LoadScriptOnJSThread");
        context_->RunScript(source.c_str());
    }

    void Runtime::LoadUrlOnJSThread(const std::string& url) {
        context_->LoadUrl(url);
    }

    void Runtime::ReloadOnJSThread(bool force) {

    }

    void Runtime::DestroyOnJSThread() {
#if ENABLE_PLUGIN
        plugin::PluginServer::UnRegister(context_->global()->plugin());
#endif
        Release();
    }

    void Runtime::SetUserAgent(const std::string& ua) {
        context_->SetUserAgent(ua);
    }

    std::string Runtime::GetUserAgent() {
        return context_->GetUserAgent();
    }

    std::string Runtime::GetPageUrl() {
        return render_tree_host()->page_location();
    }
}  // namespace jscore
