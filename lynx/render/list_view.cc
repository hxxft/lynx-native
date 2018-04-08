// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/list_view.h"
#include "render/cell_view.h"
#include "render/impl/render_command.h"
#include "render/render_tree_host.h"

namespace lynx {

ListView::ListView(const char* tag_name, uint64_t id, RenderTreeHost* host)
    : RenderObject(
          tag_name,
          LYNX_LISTVIEW,
          id,
          RenderObjectImpl::Create(host->thread_manager(), LYNX_LISTVIEW),
          host) {}

ListView::~ListView() {}

void ListView::OnLayout(int left, int top, int right, int bottom) {
  LayoutObject::OnLayout(left, top, right, bottom);

  RenderObject* child = static_cast<RenderObject*>(LastChild());
  if (child == NULL)
    return;
  if (style_.flex_direction_ == CSSStyleType::CSSFLEX_DIRECTION_COLUMN) {
    set_scroll_height(child->measured_position().bottom_);
    set_scroll_width(offset_width());
  } else {
    set_scroll_height(offset_height());
    set_scroll_width(child->measured_position().right_);
  }
}

void ListView::InsertChild(ContainerNode* child, int index) {
  RenderObject* render_object = NULL;
  if (static_cast<RenderObject*>(child)->render_object_type() ==
      LYNX_LISTSHADOW) {
    render_object = static_cast<RenderObject*>(child);

  } else {
    CellView* cell =
        lynx_new CellView(impl()->thread_manager(), render_tree_host());
    cell->InsertChild(static_cast<RenderObject*>(child), 0);
    render_object = cell;
    container_.Add(render_object);
  }

  if (!render_object)
    return;

  RenderObject::InsertChild(render_object, index);
}

void ListView::InsertBefore(RenderObject* child, RenderObject* reference) {
  int index = -1;
  if (reference != NULL) {
    index = 0;
    RenderObject* child = static_cast<RenderObject*>(FirstChild());
    while (child) {
      if (child == reference || child->Find(0) == reference) {
        break;
      }
      index++;
      child = static_cast<RenderObject*>(child->Next());
    }
  }

  InsertChild(child, index);
}

void ListView::RemoveChild(ContainerNode* child) {
  RenderObject* cell = static_cast<RenderObject*>(FirstChild());
  int index = 0;
  while (cell) {
    if (cell == child || cell->Find(0) == child) {
      break;
    }
    index++;
    cell = static_cast<RenderObject*>(cell->Next());
  }

  RenderObject::RemoveChild(cell);
  container_.Remove(cell);
}
}  // namespace lynx
