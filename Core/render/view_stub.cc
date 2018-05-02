// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/view_stub.h"

namespace lynx {

ViewStub::ViewStub(jscore::ThreadManager* manager,
                   const char* tag_name,
                   uint64_t id,
                   RenderTreeHost* host)
    : View(tag_name, LYNX_LAYOUT_VIEW, id, NULL, host) {}
}  // namespace lynx
