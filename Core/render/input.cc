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

base::Size Input::OnMeasure(int width_descriptor, int height_descriptor) {
    int widthWanted = (int) css_style_.width_;
    int heightWanted = (int) css_style_.height_;
    int widthMode = base::Size::Descriptor::GetMode(width_descriptor);
    int heightMode = base::Size::Descriptor::GetMode(height_descriptor);
    width_descriptor = base::Size::Descriptor::GetSize(width_descriptor);
    height_descriptor = base::Size::Descriptor::GetSize(height_descriptor);

    width_descriptor = CSS_IS_UNDEFINED(widthWanted)
            && !CSS_IS_UNDEFINED(width_descriptor)
            && (widthMode == base::Size::Descriptor::EXACTLY
                || widthMode == base::Size::Descriptor::AT_MOST)?
            (int) css_style_.ClampExactWidth(width_descriptor) :
            (int) css_style_.ClampWidth();
    height_descriptor = CSS_IS_UNDEFINED(heightWanted)
             && !CSS_IS_UNDEFINED(height_descriptor)
             && (heightMode == base::Size::Descriptor::EXACTLY
                 || heightMode == base::Size::Descriptor::AT_MOST)?
             (int) css_style_.ClampExactHeight(height_descriptor) :
             (int) css_style_.ClampHeight();

    width_descriptor -= css_style_.padding_left_ + css_style_.padding_right_
             + css_style_.border_width_ * 2;
    height_descriptor -= css_style_.padding_top_ + css_style_.padding_bottom_
              + css_style_.border_width_ * 2;

    base::Size size;
    if (CSS_IS_UNDEFINED(heightWanted) || CSS_IS_UNDEFINED(widthWanted)) {
        size = LabelMeasurer::MeasureLabelSize(
                this,
                base::Size(width_descriptor, height_descriptor),
                "");
        size.height_ *= default_line_count_;
        size.width_ = size.height_ * kDefaultWHRate;
    }

    size.width_ += css_style_.padding_left_ + css_style_.padding_right_
                   + css_style_.border_width_ * 2;
    size.height_ += css_style_.padding_top_ + css_style_.padding_bottom_
                    + css_style_.border_width_ * 2;

    size.width_ = (int) css_style_.ClampWidth(size.width_);
    size.height_ = (int) css_style_.ClampHeight(size.height_);


    size.width_ = !CSS_IS_UNDEFINED(width_descriptor)
                  && widthMode == base::Size::Descriptor::EXACTLY ?
                  (int) css_style_.ClampExactWidth(base::Size::Descriptor::GetSize(width_descriptor)) :
                  (int) css_style_.ClampWidth(size.width_);
    size.height_ = !CSS_IS_UNDEFINED(height_descriptor)
                   && heightMode == base::Size::Descriptor::EXACTLY ?
                   (int) css_style_.ClampExactHeight(base::Size::Descriptor::GetSize(height_descriptor)) :
                   (int) css_style_.ClampHeight(size.height_);

    measured_size_ = size;
    return size;
}

}  // namespace lynx
