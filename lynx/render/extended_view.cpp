// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/impl/render_object_impl.h"
#include "render/extended_view.h"
#include "render/render_tree_host.h"

namespace lynx {
    ExtendedView::ExtendedView(const char *tag_name,
                               RenderObjectType type,
                               uint64_t id,
                               RenderTreeHost *host)
            : RenderObject(tag_name,
                           type,
                           id,
                           RenderObjectImpl::Create(host->thread_manager(), type),
                           host) {

    }

    base::Size ExtendedView::OnMeasure(int width_descriptor, int height_descriptor) {
        return CSSStaticLayout::Measure(this, width_descriptor, height_descriptor);
    }

}