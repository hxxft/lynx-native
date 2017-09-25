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

    base::Size ExtendedView::Measure(int width, int height) {
        int widthWanted = (int) style_.width_;
        int heightWanted = (int) style_.height_;
        int widthMode = base::Size::Descriptor::GetMode(width);
        int heightMode = base::Size::Descriptor::GetMode(height);
        width = base::Size::Descriptor::GetSize(width);
        height = base::Size::Descriptor::GetSize(height);

        width = CSS_IS_UNDEFINED(widthWanted)
                && !CSS_IS_UNDEFINED(width)
                && (widthMode == base::Size::Descriptor::EXACTLY
                    || widthMode == base::Size::Descriptor::AT_MOST)?
                (int) style_.ClampExactWidth(width) :
                (int) style_.ClampWidth();
        height = CSS_IS_UNDEFINED(heightWanted)
                 && !CSS_IS_UNDEFINED(height)
                 && (heightMode == base::Size::Descriptor::EXACTLY
                     || heightMode == base::Size::Descriptor::AT_MOST)?
                 (int) style_.ClampExactHeight(height) :
                 (int) style_.ClampHeight();

        measured_size_.width_ = width;
        measured_size_.height_ = height;
        return measured_size_;
    }

}