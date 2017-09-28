// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/image_view.h"

#include "render/impl/render_object_impl.h"
#include "render/render_tree_host.h"

namespace lynx {
const static char* kImageSrcAttribute = "src";

ImageView::ImageView(const char *tag_name,
                     uint64_t id,
                     RenderTreeHost *host)
        : RenderObject(tag_name,
                       LYNX_IMAGEVIEW,
                       id,
                       RenderObjectImpl::Create(host->thread_manager(), LYNX_IMAGEVIEW),
                       host) {

}

base::Size ImageView::Measure(int width, int height) {
    if (IsDirty()) {
        if (!CSS_IS_UNDEFINED(style_.height())) {
            measured_size_.height_ = style_.height();
        }
        if (!CSS_IS_UNDEFINED(style_.width())) {
            measured_size_.width_ = style_.width();
        }
    }
    return measured_size_;
}

void ImageView::Layout(int left, int top, int right, int bottom) {
    RenderObject::Layout(left, top, right, bottom);
}

void ImageView::SetAttribute(const std::string &key, const std::string &value) {

    if(key.compare(kImageSrcAttribute) == 0) {
        RenderObject::SetAttribute(key, render_tree_host_->page_location() + value);
    }else{
        RenderObject::SetAttribute(key, value);
    }
}

}  // namespace lynx
