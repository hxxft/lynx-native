// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_LAYOUT_LAYOUT_OBJECT_H_
#define LYNX_LAYOUT_LAYOUT_OBJECT_H_

#include <string>

#include "base/position.h"
#include "base/size.h"
#include "layout/container_node.h"
#include "layout/css_style.h"

namespace lynx {
class LayoutObject : public ContainerNode {
 public:
  LayoutObject();
  virtual ~LayoutObject();

  void ReLayout(int left, int top, int right, int bottom);

  virtual void InsertChild(ContainerNode* child, int index);
  virtual void RemoveChild(ContainerNode* child);

  virtual void SetStyle(const std::string& key, const std::string& value) {
    style_.SetValue(key, value);
  }

  const CSSStyle& style() { return style_; }

  const base::Size& measured_size() { return measured_size_; }

  const base::Position& measured_position() { return measured_position_; }

  void Dirty();

  bool IsDirty();

 protected:
  enum LAYOUT_STATE {
    LAYOUT_STATE_DIRTY,
    LAYOUT_STATE_UP_TO_DATE,
  };

  void UpToDate();

  bool ShouldRemeasure(int width, int height);

  friend class CSSStaticLayout;
  virtual base::Size Measure(int width_descriptor, int height_descriptor);

  // Subclasses should override onMeasure(int, int) to provide
  // a measurements of their content.
  virtual base::Size OnMeasure(int width_descriptor, int height_descriptor);

  virtual void Layout(int left, int top, int right, int bottom);

  // Called from layout when this node should assign a position to each of its
  // children.
  virtual void OnLayout(int left, int top, int right, int bottom);

  base::Size measured_size_;
  base::Position measured_position_;

  LAYOUT_STATE layout_state_;

  CSSStyle style_;

  int offset_top_;
  int offset_left_;
  int offset_width_;
  int offset_height_;

 private:
  base::Size last_measured_size_from_parent_;
};
}  // namespace lynx

#endif  // LYNX_LAYOUT_LAYOUT_OBJECT_H_
