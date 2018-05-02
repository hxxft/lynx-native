// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_SLIDER_VIEW_H_
#define LYNX_RENDER_SLIDER_VIEW_H_

#include "render/view.h"

namespace lynx {
class SwiperView : public View {
 public:
  SwiperView(jscore::ThreadManager* manager,
             const char* tag_name,
             uint64_t id,
             RenderTreeHost* host);
  virtual ~SwiperView() {}
  virtual base::Size OnMeasure(int width, int height) override;

 protected:
  virtual void OnLayout(int left, int top, int right, int bottom) override;
};
}  // namespace lynx

#endif  // LYNX_RENDER_SLIDER_VIEW_H_
