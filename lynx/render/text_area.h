// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_TEXT_AREA_H_
#define LYNX_RENDER_TEXT_AREA_H_

#include "render/input.h"

namespace lynx {
class TextArea : public Input {
 public:
  TextArea(const char* tag_name, uint64_t id, RenderTreeHost* host);
  virtual ~TextArea() {}

 private:
  const static int kDefaultLineCount = 2;
};
}  // namespace lynx

#endif  // LYNX_RENDER_TEXT_AREA_H_
