// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/scroll_view.h"
#include "render/impl/render_command.h"
#include "render/render_object_type.h"
#include "render/render_tree_host.h"

namespace lynx {

ScrollView::ScrollView(const char* tag_name, uint64_t id, RenderTreeHost* host)
    : View(tag_name,
           LYNX_SCROLLVIEW,
           id,
           RenderObjectImpl::Create(host->thread_manager(), LYNX_SCROLLVIEW),
           host) {}

base::Size ScrollView::OnMeasure(int width, int height) {
  if (IsDirty()) {
    base::Size old_scroll_size(scroll_width(), scroll_height());
    measured_size_ = LayoutObject::OnMeasure(width, height);
    set_scroll_width(measured_size_.width_);
    set_scroll_height(measured_size_.height_);
    int width_temp = 0;
    int height_temp = 0;
    for (int i = 0; i < GetChildCount(); i++) {
      RenderObject* child = static_cast<RenderObject*>(Find(i));
      if (child->style().css_position_type_ == CSS_POSITION_ABSOLUTE ||
          child->style().css_position_type_ == CSS_POSITION_FIXED) {
        continue;
      }
      width_temp += child->measured_size().width_ +
                    child->style().margin_right() +
                    child->style().margin_left();
      height_temp += child->measured_size().height_ +
                     child->style().margin_bottom() +
                     child->style().margin_top();
    }
    if (style_.flex_direction() == CSSFLEX_DIRECTION_ROW) {
      set_scroll_width(width_temp);
    } else {
      set_scroll_height(height_temp);
    }
    if (old_scroll_size.Update(scroll_width(), scroll_height())) {
      base::Size size(scroll_width(), scroll_height());
      RenderCommand* cmd = lynx_new RendererSizeUpdateCommand(
          impl(), size, RenderCommand::CMD_SET_SIZE);
      render_tree_host()->UpdateRenderObject(cmd);
    }
  }
  return measured_size_;
}

void ScrollView::OnLayout(int left, int top, int right, int bottom) {
  LayoutObject::OnLayout(left, top, right, bottom);
}

}  // namespace lynx
