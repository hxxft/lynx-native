// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/impl/render_object_impl.h"
#include "render/text_area.h"
#include "render/label_measurer.h"
#include "render/render_tree_host.h"

namespace lynx {

    TextArea::TextArea(const char *tag_name,
                       uint64_t id,
                       RenderTreeHost *host)
            : Input(tag_name,
                    LYNX_TEXT_AREA,
                    id,
                    RenderObjectImpl::Create(host->thread_manager(), LYNX_TEXT_AREA),
                    host,
                    kDefaultLineCount) {

    }
}