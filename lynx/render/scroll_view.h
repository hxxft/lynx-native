// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_SCROLL_VIEW_H_
#define LYNX_RENDER_SCROLL_VIEW_H_

#include "render/render_object.h"

#include "render/view.h"

namespace lynx {

class ScrollView : public View {
 public:
    ScrollView(const char* tag_name, uint64_t id, RenderTreeHost* host);
    virtual ~ScrollView() {}
    virtual base::Size Measure(int width, int height);
    virtual void Layout(int left, int top, int right, int bottom);
};
}  // namespace lynx

#endif  // LYNX_RENDER_SCROLL_VIEW_H_
