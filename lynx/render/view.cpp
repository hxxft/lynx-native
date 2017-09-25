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

base::Size View::Measure(int width, int height) {
    return RenderObject::Measure(width, height);
}

void View::Layout(int left, int top, int right, int bottom) {
    RenderObject::Layout(left, top, right, bottom);
}
}  // namespace lynx
