// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_VIEW_H_
#define LYNX_RENDER_VIEW_H_

#include "render/render_object.h"
#include "render/impl/render_object_impl.h"

namespace lynx {

class View : public RenderObject {
 public:

    View(const char* tag_name,
         uint64_t id,
         RenderObjectImpl* proxy,
         RenderTreeHost* host);

    View(const char* tag_name,
         RenderObjectType type,
         uint64_t id,
         RenderObjectImpl* proxy,
         RenderTreeHost* host);

    virtual ~View() {}
    virtual base::Size Measure(int width, int height);
    virtual void Layout(int left, int top, int right, int bottom);
};

}  // namespace lynx

#endif  // LYNX_RENDER_VIEW_H_
