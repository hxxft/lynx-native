// Copyright 2017 The Lynx Authors. All rights reserved.

#include <base/print.h>
#include "runtime/js_context.h"
#include "runtime/runtime.h"
#include "runtime/js_vm.h"

#include "loader/html/html_loader.h"
#include "parser/render_parser.h"
#include "render/render_tree_host.h"
#include "render/render_tree_host_impl.h"
#include "runtime/base/lynx_function_object.h"
#include "loader/utils.h"

namespace jscore {

    Runtime::Runtime(JSContext* context)
            : thread_manager_(lynx_new ThreadManager()),
              url_request_context_(lynx_new net::URLRequestContext(this)),
              loader_(lynx_new loader::HTMLLoader(this)),
              context_(context),
              weak_ptr_(this) {
                
    }
    
    void Runtime::InitRuntime(const char* arg) {
        thread_manager_->RunOnJSThread(
                base::Bind(&Runtime::InitRuntimeOnJSThread, weak_ptr_, arg));
    }

    lynx::RenderTreeHost* Runtime::SetupRenderHost() {
        lynx::Body* root = lynx_new lynx::Body(thread_manager(), render_tree_host());
        render_tree_host_ = lynx_new lynx::RenderTreeHost(context_.Get(),
                                                     thread_manager_.Get(), root);
        root->SetHost(render_tree_host_.Get());
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
                base::Bind(&Runtime::LoadUrlOnJSThread, weak_ptr_, url));
    }

    void Runtime::LoadScript(const std::string& url, int type) {
        std::string transformed_url = render_tree_host_->page_location() + url;
        loader_->Load(transformed_url, type);
    }

    void Runtime::Reload(bool force) {
        thread_manager_->RunOnJSThread(
                base::Bind(&Runtime::ReloadOnJSThread, weak_ptr_, force));
    }

    void Runtime::RunScript(const base::PlatformString& source) {
        thread_manager_->RunOnJSThread(
                base::Bind(&Runtime::RunScriptOnJSThread, weak_ptr_, source));
    }


    void Runtime::LoadHTML(const std::string& url, const std::string& html) {
        render_tree_host_->set_page_location(url);
        LoadHTML(html);
    }

    void Runtime::LoadHTML(const std::string& html) {
        parser::RenderParser parser(render_tree_host(), this);
        parser.Insert(html);
        render_tree_host()->ForceFlushCommands();
        render_tree_host()->host_impl()->SetParseFinished();
    }

    void Runtime::LoadScript(const std::string& source) {
        LoadScriptOnJSThread(source);
    }

    void Runtime::FlushScript() {
        loader_->Flush();
    }

    void Runtime::AddJavaScriptInterface(const std::string& name, LynxFunctionObject* object) {
        base::ScopedPtr<LynxFunctionObject> scoped_ptr(object);
        thread_manager_->RunOnJSThread(
                base::Bind(&Runtime::AddJavaScriptInterfaceOnJSThread, weak_ptr_, name, scoped_ptr));
    }

    void Runtime::AddJavaScriptInterfaceOnJSThread(const std::string& name,
                                                   base::ScopedPtr<LynxFunctionObject> object) {
        context_->AddJavaScriptInterface(name, object.Release());
    }

    void Runtime::Destroy() {
        url_request_context_->Stop();
        thread_manager_->DetachUIThread();
        thread_manager_->QuitJSThread(base::Bind(&Runtime::DestroyOnJSThread, weak_ptr_));
    }

    void Runtime::InitRuntimeOnJSThread(const char *arg) {
        vm_ = lynx_new JSVM();
        vm_->Initialize();
        context_->Initialize(vm_.Get(), this);
    }

    void Runtime::RunScriptOnJSThread(const base::PlatformString& source) {
        context_->RunScript(const_cast<base::PlatformString*>(&source)->GetUTFChars());
    }

    void Runtime::LoadScriptOnJSThread(const std::string& source) {
        context_->RunScript(source.c_str());
    }

    void Runtime::LoadUrlOnJSThread(const std::string& url) {
        context_->LoadUrl(url);
    }

    void Runtime::ReloadOnJSThread(bool force) {

    }

    void Runtime::DestroyOnJSThread() {
        lynx_delete(this);
    }

    std::string Runtime::GetUserAgent() {
        return context_->GetUserAgent();
    }

    std::string Runtime::GetPageUrl() {
        return render_tree_host()->page_location();
//        return context_->GetPageUrl();
    }
}  // namespace jscore
