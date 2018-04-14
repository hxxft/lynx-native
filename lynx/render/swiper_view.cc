// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/swiper_view.h"
#include <math.h>
#include "render/impl/render_object_impl.h"

namespace lynx {
SwiperView::SwiperView(jscore::ThreadManager* manager,
                       const char* tag_name,
                       uint64_t id,
                       RenderTreeHost* host)
    : View(tag_name,
           LYNX_SWIPER,
           id,
           RenderObjectImpl::Create(manager, LYNX_SWIPER),
           host) {}

base::Size SwiperView::OnMeasure(int width, int height) {
    
  measured_size_.height_ = 0;
  measured_size_.width_ = 0;
  base::Size measured_size;
  for (int i = 0; i < GetChildCount(); i++) {
    measured_size = CSSStaticLayout::Measure(
        static_cast<LayoutObject*>(Find(i)), width, height);
    measured_size_.height_ =
        std::max(measured_size_.height_, measured_size.height_);
    measured_size_.width_ =
        std::max(measured_size_.width_, measured_size.width_);
  }
  measured_size_.width_ = (int)style_.ClampWidth(measured_size_.width_);
  measured_size_.height_ = (int)style_.ClampHeight(measured_size_.height_);
  return measured_size_;
}

void SwiperView::OnLayout(int left, int top, int right, int bottom) {
    
    for (int i = 0; i < GetChildCount(); i++) {
        static_cast<LayoutObject*>(Find(i))->OnLayout(left, top, right, bottom);
    }
}

}  // namespace lynx
