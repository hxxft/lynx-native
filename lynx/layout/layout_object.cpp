// Copyright 2017 The Lynx Authors. All rights reserved.

#include "layout/layout_object.h"

namespace lynx {

LayoutObject::LayoutObject() :
    measured_size_(),
    measured_position_(),
    layout_state_(LAYOUT_STATE_DIRTY),
    style_() {
}

LayoutObject::~LayoutObject() {
}

void LayoutObject::InsertChild(ContainerNode* child, int index) {
    ContainerNode::InsertChild(child, index);
    Dirty();
}

void LayoutObject::RemoveChild(ContainerNode* child) {
    ContainerNode::RemoveChild(child);
    Dirty();
}

base::Size LayoutObject::Measure(int width_descriptor, int height_descriptor) {
    if (ShouldRemeasure(width_descriptor, height_descriptor) || IsDirty()) {
        measured_size_ = OnMeasure(width_descriptor, height_descriptor);
    }
    return measured_size_;
}

base::Size LayoutObject::OnMeasure(int width_descriptor, int height_descriptor) {
    return base::Size(0, 0);
}

void LayoutObject::Layout(int left, int top, int right, int bottom) {
    UpToDate();
    if (measured_position_.Reset(left, top, right, bottom) || IsDirty()) {
        OnLayout(left, top, right, bottom);
    }
}

void LayoutObject::OnLayout(int left, int top, int right, int bottom) {
}

void LayoutObject::Dirty() {
    LayoutObject* parent = static_cast<LayoutObject*>(parent_);
    if (parent != NULL &&
            parent->layout_state_ != LAYOUT_STATE_DIRTY) {
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

bool LayoutObject::ShouldRemeasure(int width_descriptor, int height_descriptor) {
    return last_measured_size_from_parent_.Update(width_descriptor, height_descriptor);
}
}  // namespace lynx