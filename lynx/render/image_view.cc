// Copyright 2017 The Lynx Authors. All rights reserved.

#include <base/log/logging.h>
#include "render/image_view.h"

#include "render/impl/render_object_impl.h"
#include "render/render_tree_host.h"

namespace lynx {
const static char* kImageSrcAttribute = "src";
const std::string kAssets = "Asset://assets/";

ImageView::ImageView(const char *tag_name,
                     uint64_t id,
                     RenderTreeHost *host)
        : RenderObject(tag_name,
                       LYNX_IMAGEVIEW,
                       id,
                       RenderObjectImpl::Create(host->thread_manager(), LYNX_IMAGEVIEW),
                       host) {

}

base::Size ImageView::OnMeasure(int width_descriptor, int height_descriptor) {
    DLOG(INFO) << style_.width() << " "<<style_.height();
    if (!CSS_IS_UNDEFINED(style_.height())) {
        measured_size_.height_ = style_.height();
    }
    if (!CSS_IS_UNDEFINED(style_.width())) {
        measured_size_.width_ = style_.width();
    }
    return measured_size_;
}

void ImageView::SetAttribute(const std::string &key, const std::string &value) {

    if(key.compare(kImageSrcAttribute) == 0) {
        if (value.find("http:") != std::string::npos
            || value.find("https:") != std::string::npos) {
            RenderObject::SetAttribute(key, value);
        } else {
            RenderObject::SetAttribute(key, kAssets + value);
        }
    } else {
        RenderObject::SetAttribute(key, value);
    }

}

}  // namespace lynx
