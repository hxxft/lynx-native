// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/debug/memory_debug.h"
#include "render/body.h"
#include "render/impl/render_command.h"
#include "render/render_object_type.h"

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
        View::Measure(body_size_.width_, body_size_.height_);
        View::Layout(0, 0, body_size_.width_, body_size_.height_);
    }
}

void Body::Layout(int left, int top, int right, int bottom) {
    if (measured_position_.Reset(left, top, right, bottom)) {
        Dirty();
    }
    if (IsDirty()) {
        int width = right - left;
        int height = bottom - top;
        View::Measure(width, height);
        View::Layout(0, 0, width, height);
    }
}

void Body::LayoutWithTick(int tick) {
    Layout();
    thread_manager_->RunOnJSThreadDelay(
        base::Bind(&Body::LayoutWithTick, weak_ptr_, tick), tick);
}

}  // namespace lynx
