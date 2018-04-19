// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/impl/render_object_impl.h"

#include "render/render_object.h"

namespace lynx {

RenderObjectImpl::RenderObjectImpl(
    jscore::ThreadManager* manager,
    RenderObjectType type)
    : thread_manager_(manager),
      render_object_type_(type) {
}

RenderObjectImpl::~RenderObjectImpl() {
}


base::ScopedPtr<jscore::LynxValue> RenderObjectImpl::GetLynxValue() {
    if (render_object_weak_ptr_.IsValid()) {
        return jscore::LynxValue::MakeObjectTemplate(render_object_weak_ptr_->GetJSRef());
    } else {
        return jscore::LynxValue::MakeObjectTemplate(NULL);
    }
}

}  // namespace lynx
