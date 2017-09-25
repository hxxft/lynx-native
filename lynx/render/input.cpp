// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/label_measurer.h"
#include "render/input.h"
#include "render/impl/render_object_impl.h"
#include "render/render_tree_host.h"

namespace lynx {
Input::Input(const char *tag_name,
             uint64_t id,
             RenderTreeHost* host)
    : RenderObject(tag_name,
                   LYNX_INPUT,
                   id,
                   RenderObjectImpl::Create(host->thread_manager(), LYNX_INPUT),
                   host),
      default_line_count_(kDefaultLineCount) {
}

Input::Input(const char *tag_name,
             RenderObjectType type,
             uint64_t id,
             RenderObjectImpl *proxy,
             RenderTreeHost *host,
             int default_line_count)
        : RenderObject(tag_name,
                       type,
                       id,
                       proxy,
                       host),
          default_line_count_(
                  default_line_count) {

}

base::Size Input::Measure(int width, int height) {
    if (!IsDirty()) return measured_size_;
    int widthWanted = (int) style_.width_;
    int heightWanted = (int) style_.height_;
    int widthMode = base::Size::Descriptor::GetMode(width);
    int heightMode = base::Size::Descriptor::GetMode(height);
    width = base::Size::Descriptor::GetSize(width);
    height = base::Size::Descriptor::GetSize(height);

    width = CSS_IS_UNDEFINED(widthWanted)
            && !CSS_IS_UNDEFINED(width)
            && (widthMode == base::Size::Descriptor::EXACTLY
                || widthMode == base::Size::Descriptor::AT_MOST)?
            (int) style_.ClampExactWidth(width) :
            (int) style_.ClampWidth();
    height = CSS_IS_UNDEFINED(heightWanted)
             && !CSS_IS_UNDEFINED(height)
             && (heightMode == base::Size::Descriptor::EXACTLY
                 || heightMode == base::Size::Descriptor::AT_MOST)?
             (int) style_.ClampExactHeight(height) :
             (int) style_.ClampHeight();

    width -= style_.padding_left_ + style_.padding_right_
             + style_.border_width_ * 2;
    height -= style_.padding_top_ + style_.padding_bottom_
              + style_.border_width_ * 2;

    base::Size size;
    if (CSS_IS_UNDEFINED(heightWanted) || CSS_IS_UNDEFINED(widthWanted)) {
        size = LabelMeasurer::MeasureLabelSize(
                this,
                base::Size(width, height),
                "");
        size.height_ *= default_line_count_;
        size.width_ = size.height_ * kDefaultWHRate;
    }

    size.width_ += style_.padding_left_ + style_.padding_right_
                   + style_.border_width_ * 2;
    size.height_ += style_.padding_top_ + style_.padding_bottom_
                    + style_.border_width_ * 2;

    size.width_ = (int) style_.ClampWidth(size.width_);
    size.height_ = (int) style_.ClampHeight(size.height_);


    size.width_ = !CSS_IS_UNDEFINED(width)
                  && widthMode == base::Size::Descriptor::EXACTLY ?
                  (int) style_.ClampExactWidth(base::Size::Descriptor::GetSize(width)) :
                  (int) style_.ClampWidth(size.width_);
    size.height_ = !CSS_IS_UNDEFINED(height)
                   && heightMode == base::Size::Descriptor::EXACTLY ?
                   (int) style_.ClampExactHeight(base::Size::Descriptor::GetSize(height)) :
                   (int) style_.ClampHeight(size.height_);

    measured_size_ = size;
    return size;
}

}  // namespace lynx
