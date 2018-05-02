// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/list_shadow.h"
#include "render/cell_view.h"
#include "render/render_tree_host.h"

namespace lynx {
ListShadow::ListShadow(const char* tag_name, uint64_t id, RenderTreeHost* host)
    : View(tag_name,
           LYNX_LISTSHADOW,
           id,
           RenderObjectImpl::Create(host->thread_manager(), LYNX_LISTSHADOW),
           host) {
  css_style_.set_flex_direction(CSSFLEX_DIRECTION_COLUMN);
}

void ListShadow::OnLayout(int left, int top, int right, int bottom) {
  LayoutObject::OnLayout(left, top, right, bottom);
}

void ListShadow::InsertChild(ContainerNode* child, int index) {
  CellView* cell =
      lynx_new CellView(impl()->thread_manager(), render_tree_host());
  cell->InsertChild(static_cast<RenderObject*>(child), 0);

  View::InsertChild(cell, index);
  container_.Add(cell);
}

void ListShadow::InsertBefore(RenderObject* child, RenderObject* reference) {
  int index = -1;
  if (reference != NULL) {
    index = 0;
    RenderObject* renderer = static_cast<RenderObject*>(FirstChild());
    while (renderer) {
      if (renderer->GetChildCount() > 0 && renderer->Find(0) == reference) {
        break;
      }
      index++;
      renderer = static_cast<RenderObject*>(renderer->Next());
    }
  }
  InsertChild(child, index);
}

void ListShadow::RemoveChild(ContainerNode* child) {
  RenderObject* cell = static_cast<RenderObject*>(FirstChild());
  while (cell) {
    if (cell->GetChildCount() > 0 && cell->Find(0) == child) {
      break;
    }
    cell = static_cast<RenderObject*>(cell->Next());
  }
  View::RemoveChild(cell);
  container_.Remove(cell);
}

}  // namespace lynx
