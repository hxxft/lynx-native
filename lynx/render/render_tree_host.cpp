// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/render_tree_host.h"

#include "runtime/element.h"
#include "runtime/runtime.h"
#include "render/render_object.h"
#include "render/render_tree_host_impl.h"
#include "render/impl/render_command.h"

#if OS_ANDROID
#include "render/android/render_tree_host_impl_android.h"
#elif OS_IOS
#include "render/ios/render_tree_host_impl_ios.h"
#endif


#include "base/trace_event/trace_event_common.h"

namespace lynx {

RenderTreeHost::RenderTreeHost(
        jscore::JSContext* context,
    jscore::ThreadManager* thread_manager,
    RenderObject* root)
    : weak_ptr_(this),
      collector_(),
      context_(context),
      thread_manager_(thread_manager),
      render_root_(root),
      did_first_layout_(false),
      page_location_("") {

#if OS_ANDROID
    render_tree_host_impl_ = lynx_new RenderTreeHostImplAndroid(
        thread_manager,
        this,
        root->impl());
#elif OS_IOS
    render_tree_host_impl_ = lynx_new RenderTreeHostImplIOS(
        thread_manager,
        this,
        render_root_->impl());
#endif
}

RenderTreeHost::~RenderTreeHost() {
    weak_ptr_.Invalidate();
}

void RenderTreeHost::UpdateRenderObject(RenderCommand* command) {
    // exec command immediately
    //thread_manager_->RunOnUIThread(command);

    // collect command and wait vsync signal to exec
    collector_.Collect(command);
}

void RenderTreeHost::ForceLayout(int left, int top, int right, int bottom) {
    render_root_->Layout(left, top, right, bottom);
}

void RenderTreeHost::DoBeginFrame(const BeginFrameData& data) {
    view_port_ = data.viewport_;
    PrepareCommit(data);
    render_root_->Layout(0, 0, data.viewport_.width_, data.viewport_.height_);
    render_tree_host_impl_->NotifyBeginFrameComplete();
}

void RenderTreeHost::PrepareCommit(const BeginFrameData& data) {
    render_tree_host_impl_->PrepareCommit();
    collector_.Push();
}

void RenderTreeHost::DoCommit() {
    TRACE_EVENT0("renderer", "RenderTreeHost::DoCommit");
    RenderCommandCollector::RenderCommands *commands = collector_.Pop();
    if (commands == NULL)
        return;
    RenderCommandCollector::RenderCommandsStorage::iterator iter =
        commands->begin();
    for (iter; iter != commands->end(); ++iter) {
        (*iter)->Execute();
    }
    lynx_delete(commands);
}

void RenderTreeHost::ForceFlushCommands() {
    TRACE_EVENT0("js", "RenderTreeHost::ForceFlushCommands");
    collector_.Push();
    DoCommit();
}

void RenderTreeHost::TreeSync() {
  {
    TRACE_EVENT0("js", "RenderTreeHost::TreeSync");
    RendererSync(render_root());
  }
  context_->runtime()->FlushScript();
}

void RenderTreeHost::RendererSync(RenderObject* renderer) {
    RenderObject* child = static_cast<RenderObject*>(renderer->FirstChild());
    while(child) {
        if(!child->IsPrivate()) {
            jscore::Element *element = lynx_new jscore::Element(context_, child);
            jscore::Element::ProtectChild(context_, element);
        }
        RendererSync(child);
        child = static_cast<RenderObject*>(child->Next());
    }
}

}  // namespace lynx
