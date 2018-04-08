// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_SCROLL_VIEW_H_
#define LYNX_RENDER_SCROLL_VIEW_H_

#include "render/render_object.h"

#include "render/view.h"

namespace lynx {

class ScrollView : public View {
 public:
  ScrollView(const char* tag_name, uint64_t id, RenderTreeHost* host);
  virtual ~ScrollView() {}
  virtual base::Size OnMeasure(int width, int height) override;

 protected:
  virtual void OnLayout(int left, int top, int right, int bottom) override;
};
}  // namespace lynx

#endif  // LYNX_RENDER_SCROLL_VIEW_H_
