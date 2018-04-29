// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/cell_view.h"
#include "render/render_object_type.h"

namespace lynx {

CellView::CellView(jscore::ThreadManager* manager, RenderTreeHost* host)
    : View("cell",
           LYNX_CELLVIEW,
           1,
           RenderObjectImpl::Create(manager, LYNX_CELLVIEW),
           host) {
  css_style_.set_flex_direction(CSSFLEX_DIRECTION_COLUMN);
}

void CellView::Layout(int left, int top, int right, int bottom) {
  RenderObject::Layout(0, 0, right - left, bottom - top);
}

void CellView::OnLayout(int left, int top, int right, int bottom) {
  LayoutObject::OnLayout(0, 0, right - left, bottom - top);
  if (GetChildCount() > 0) {
    RenderObject* child = static_cast<RenderObject*>(Find(0));
    if (child->css_style().css_position_type_ != CSSStyleType::CSS_POSITION_FIXED) {
      child->set_offset_top(top);
      child->set_offset_left(left);
    }
  }
}
}  // namespace lynx
