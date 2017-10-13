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

base::Size LayoutObject::Measure(int width, int height) {
    if (ShouldRemeasure(width, height) || IsDirty()) {
        measured_size_ = CSSStaticLayout::Measure(this, width, height);
        return measured_size_;
    }
    return measured_size_;
}

void LayoutObject::Layout(int left, int top, int right, int bottom) {
    if (measured_position_.Update(left, top, right, bottom) || IsDirty()) {
        CSSStaticLayout::Layout(this, right - left, bottom - top);
        layout_state_ = LAYOUT_STATE_UP_TO_DATE;
    }
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

bool LayoutObject::ShouldRemeasure(int width, int height) {
    if (last_measured_height_from_parent_ != height
            || last_measured_width_from_parent_ != width) {
        last_measured_height_from_parent_ = height;
        last_measured_width_from_parent_ = width;
        return true;
    }
    return false;
}
}  // namespace lynx