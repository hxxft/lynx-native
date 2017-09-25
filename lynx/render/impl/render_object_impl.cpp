// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/impl/render_object_impl.h"

#if IOS
#include "render/ios/render_object_impl_ios.h"
#else
#include "render/android/render_object_impl_android.h"
#endif

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
#if IOS
    return lynx_new RenderObjectImplIOS(proxy, type);
#else
    return lynx_new RenderObjectImplAndroid(proxy, type);
#endif
}
}  // namespace lynx
