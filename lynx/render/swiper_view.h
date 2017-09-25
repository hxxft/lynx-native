// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_SLIDER_VIEW_H_
#define LYNX_RENDER_SLIDER_VIEW_H_

#include "render/view.h"

namespace lynx {
class SwiperView : public View {
 public:
    SwiperView(jscore::ThreadManager* manager,
               const char *tag_name,
               uint64_t id,
               RenderTreeHost* host);
    virtual ~SwiperView() {}
    virtual base::Size Measure(int width, int height);
    virtual void Layout(int left, int top, int right, int bottom);
};
}  // namespace lynx

#endif  // LYNX_RENDER_SLIDER_VIEW_H_
