// Copyright 2017 The Lynx Authors. All rights reserved.

#include "layout/layout_object.h"

#ifndef TESTING
#include "base/trace_event/trace_event_common.h"
#endif
namespace lynx {

LayoutObject::LayoutObject()
    : measured_size_(),
      measured_position_(),
      layout_state_(LAYOUT_STATE_DIRTY),
      style_(),
      offset_top_(0),
      offset_left_(0),
      offset_width_(0),
      offset_height_(0),
      last_measured_size_from_parent_() {}

LayoutObject::~LayoutObject() {}

void LayoutObject::InsertChild(ContainerNode* child, int index) {
  ContainerNode::InsertChild(child, index);
  Dirty();
}

void LayoutObject::RemoveChild(ContainerNode* child) {
  ContainerNode::RemoveChild(child);
  Dirty();
}

base::Size LayoutObject::Measure(int width_descriptor, int height_descriptor) {
  if (NeedRemeasure(width_descriptor, height_descriptor)) {
    measured_size_ = OnMeasure(width_descriptor, height_descriptor);
  }
  return measured_size_;
}

base::Size LayoutObject::OnMeasure(int width_descriptor,
                                   int height_descriptor) {
  return CSSStaticLayout::Measure(this, width_descriptor, height_descriptor);
}

void LayoutObject::Layout(int left, int top, int right, int bottom) {
  if (measured_position_.Reset(left, top, right, bottom) || IsDirty()) {
    offset_top_ =
        top -
        (parent_ == NULL ? 0 : ((LayoutObject*)parent_)->style_.border_width_);
    offset_left_ =
        left -
        (parent_ == NULL ? 0 : ((LayoutObject*)parent_)->style_.border_width_);
    offset_height_ = bottom - top;
    offset_width_ = right - left;
    UpToDate();
    OnLayout(left, top, right, bottom);
  }
}

void LayoutObject::OnLayout(int left, int top, int right, int bottom) {
  CSSStaticLayout::Layout(this, right - left, bottom - top);
}

void LayoutObject::ReLayout(int left, int top, int right, int bottom) {
  if (!measured_position_.Equal(left, top, right, bottom)) {
    Dirty();
  }
  if (IsDirty()) {
#ifndef TESTING
    TRACE_EVENT0("Layout", "LayoutObject::Layout");
#endif
    int width = right - left;
    int height = bottom - top;
    Measure(width, height);
    Layout(left, top, right, bottom);
  }
}

void LayoutObject::Dirty() {
  LayoutObject* parent = static_cast<LayoutObject*>(parent_);
  if (parent != NULL && parent->layout_state_ != LAYOUT_STATE_DIRTY) {
    parent->Dirty();
  }
  layout_state_ = LAYOUT_STATE_DIRTY;
}

void LayoutObject::UpToDate() {
  layout_state_ = LAYOUT_STATE_UP_TO_DATE;
}

bool LayoutObject::IsDirty() {
  return layout_state_ == LAYOUT_STATE_DIRTY;
}

bool LayoutObject::NeedRemeasure(int width_descriptor, int height_descriptor) {
  return last_measured_size_from_parent_.Update(width_descriptor,
                                                height_descriptor) ||
         IsDirty();
}
}  // namespace lynx
