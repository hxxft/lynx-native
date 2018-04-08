// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_INPUT_H_
#define LYNX_RENDER_INPUT_H_

#include "render/render_object.h"

namespace lynx {
class Input : public RenderObject {
 public:
  Input(const char* tag_name, uint64_t id, RenderTreeHost* host);

  Input(const char* tag_name,
        RenderObjectType type,
        uint64_t id,
        RenderObjectImpl* proxy,
        RenderTreeHost* host,
        int default_line_count);

  virtual ~Input() {}

 protected:
  virtual base::Size OnMeasure(int width_descriptor,
                               int height_descriptor) override;

 private:
  int default_line_count_;
  const static int kDefaultLineCount = 1;
  const static int kDefaultWHRate = 4 / 1;
};
}  // namespace lynx

#endif  // LYNX_RENDER_INPUT_H_
