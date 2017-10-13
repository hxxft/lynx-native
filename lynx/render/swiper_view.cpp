// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/swiper_view.h"
#include <math.h>
#include "render/impl/render_object_impl.h"

namespace lynx {
SwiperView::SwiperView(
    jscore::ThreadManager *manager,
    const char *tag_name,
    uint64_t id,
    RenderTreeHost *host)
    : View(
          tag_name,
          LYNX_SWIPER,
          id,
          RenderObjectImpl::Create(manager, LYNX_SWIPER), host) {
}

base::Size SwiperView::Measure(int width, int height) {
    if (!ShouldRemeasure(width, height) || !IsDirty())
        return measured_size_;
    measured_size_.height_ = 0;
    measured_size_.width_ = 0;
    base::Size measured_size;
    for(int i = 0; i < GetChildCount(); i++) {
        measured_size = static_cast<RenderObject*>(Find(i))->Measure(width, height);
        measured_size_.height_ = std::max(measured_size_.height_, measured_size.height_);
        measured_size_.width_ = std::max(measured_size_.width_, measured_size.width_);
    }
    measured_size_.width_ = (int) style_.ClampWidth(measured_size_.width_);
    measured_size_.height_ = (int) style_.ClampHeight(measured_size_.height_);
    return measured_size_;
}

void SwiperView::Layout(int left, int top, int right, int bottom) {
    RenderObject::Layout(left, top, right, bottom);
    if (IsDirty()) {
        for(int i = 0; i < GetChildCount(); i++) {
            static_cast<RenderObject*>(Find(i))->Layout(left, top, right, bottom);
        }
    }
}

}  // namespace lynx

