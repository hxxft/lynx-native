// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_IMAGE_VIEW_H_
#define LYNX_RENDER_IMAGE_VIEW_H_

#include "render/render_object.h"

namespace lynx {
class ImageView : public RenderObject {
 public:
    ImageView(const char* tag_name,
              uint64_t id,
              RenderTreeHost* host);
    virtual ~ImageView() {}
    virtual base::Size OnMeasure(int width_descriptor, int height_descriptor);
    virtual void SetAttribute(const std::string& key, const std::string& value);
};
}  // namespace lynx

#endif  // LYNX_RENDER_IMAGE_VIEW_H_
