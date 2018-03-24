// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/debug/memory_debug.h"
#include "render/body.h"
#include "render/impl/render_command.h"
#include "render/render_object_type.h"

#include "base/trace_event/trace_event_common.h"
namespace lynx {

Body::Body(jscore::ThreadManager* manager, RenderTreeHost* host):
    View("body", LYNX_BODY, -1,
        RenderObjectImpl::Create(manager, LYNX_BODY),
        host),
    thread_manager_(manager),
    body_size_(),
    did_first_layout_(false),
    weak_ptr_(this) {
}

void Body::Layout() {
    if (IsDirty()) {
        //TRACE_EVENT0("Layout", "Body::Layout");
        View::Measure(body_size_.width_, body_size_.height_);
        View::Layout(0, 0, body_size_.width_, body_size_.height_);
    }
}

void Body::Layout(int left, int top, int right, int bottom) {
    if (measured_position_.NeedToReset(left, top, right, bottom)) {
        Dirty();
    }
    if (IsDirty()) {
        TRACE_EVENT0("Layout", "Body::Layout");
        int width = right - left;
        int height = bottom - top;
        View::Measure(width, height);
        View::Layout(left, top, right, bottom);
    }
}

void Body::LayoutWithTick(int tick) {
    Layout();
    thread_manager_->RunOnJSThreadDelay(
        base::Bind(&Body::LayoutWithTick, weak_ptr_, tick), tick);
}

}  // namespace lynx
