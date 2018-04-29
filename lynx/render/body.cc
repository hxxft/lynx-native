// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/body.h"
#include "base/debug/memory_debug.h"
#include "render/impl/render_command.h"
#include "render/render_object_type.h"

namespace lynx {

Body::Body(jscore::ThreadManager* manager, RenderTreeHost* host)
    : View("body",
           LYNX_BODY,
           9,
           RenderObjectImpl::Create(manager, LYNX_BODY),
           host),
      thread_manager_(manager),
      body_size_(),
      did_first_layout_(false),
      weak_ptr_(this) {}

void Body::LayoutWithTick(int tick) {
  LayoutObject::ReLayout(0, 0, body_size_.width_, body_size_.height_);
  thread_manager_->RunOnJSThreadDelay(
      base::Bind(&Body::LayoutWithTick, weak_ptr_, tick), tick);
}

}  // namespace lynx
