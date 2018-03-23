// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/view.h"
#include "render/render_tree_host.h"

namespace lynx {

View::View(const char *tag_name,
           RenderObjectType type,
           uint64_t id,
           RenderObjectImpl *proxy,
           RenderTreeHost *host)
        : RenderObject(tag_name,
                       type,
                       id,
                       proxy ? proxy : RenderObjectImpl::Create(
                               host->thread_manager(),
                               LYNX_VIEW),
                       host) {

}

View::View(const char *tag_name,
           uint64_t id,
           RenderObjectImpl *proxy,
           RenderTreeHost *host)
        : RenderObject(tag_name,
                       LYNX_VIEW,
                       id,
                       proxy ? proxy : RenderObjectImpl::Create(
                               host->thread_manager(),
                               LYNX_VIEW),
                       host) {

}

base::Size View::OnMeasure(int width_descriptor, int height_descriptor) {
    return CSSStaticLayout::Measure(this, width_descriptor, height_descriptor);
}

void View::OnLayout(int left, int top, int right, int bottom) {
    CSSStaticLayout::Layout(this, right - left, bottom - top);
}
}  // namespace lynx
