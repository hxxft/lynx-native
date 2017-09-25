// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_LAYOUT_VIEW_H_
#define LYNX_RENDER_LAYOUT_VIEW_H_

#include "render/view.h"

namespace lynx {
class ViewStub : public View {
 public:
    ViewStub(jscore::ThreadManager* manager,
               const char *tag_name,
               uint64_t id,
               RenderTreeHost* host);
};
}

#endif  // LYNX_RENDER_LAYOUT_VIEW_H_
