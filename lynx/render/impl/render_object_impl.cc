// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/impl/render_object_impl.h"

#if OS_IOS
#include "render/ios/render_object_impl_ios.h"
#else
#include "render/android/render_object_impl_android.h"
#endif
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


RenderObjectImpl* RenderObjectImpl::Create(jscore::ThreadManager *proxy,
        RenderObjectType type) {
#if OS_IOS
    return lynx_new RenderObjectImplIOS(proxy, type);
#else
    return lynx_new RenderObjectImplAndroid(proxy, type);
#endif
}

base::ScopedPtr<jscore::LynxValue> RenderObjectImpl::GetLynxValue() {
    if (render_object_weak_ptr_.IsValid()) {
        return jscore::LynxValue::MakeObjectTemplate(render_object_weak_ptr_->GetJSRef());
    } else {
        return jscore::LynxValue::MakeObjectTemplate(NULL);
    }
}

}  // namespace lynx
