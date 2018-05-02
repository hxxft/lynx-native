// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_VIEW_H_
#define LYNX_RENDER_VIEW_H_

#include "render/impl/render_object_impl.h"
#include "render/render_object.h"

namespace lynx {

class View : public RenderObject {
 public:
  View(const char* tag_name,
       uint64_t id,
       RenderObjectImpl* proxy,
       RenderTreeHost* host);

  View(const char* tag_name,
       RenderObjectType type,
       uint64_t id,
       RenderObjectImpl* proxy,
       RenderTreeHost* host);

  virtual ~View() {}
  virtual base::Size OnMeasure(int width_descriptor,
                               int height_descriptor) override;

 protected:
  virtual void OnLayout(int left, int top, int right, int bottom) override;
};

}  // namespace lynx

#endif  // LYNX_RENDER_VIEW_H_
