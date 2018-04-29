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
    css_style_.SetValue(key, value);
  }

  void set_css_style(const CSSStyle& css_style) { css_style_ = css_style; }

  const CSSStyle& css_style() { return css_style_; }

  const base::Size& measured_size() { return measured_size_; }

  const base::Position& measured_position() { return measured_position_; }

  void Dirty();

  bool IsDirty();

  inline void set_offset_top(int offset_top) { offset_top_ = offset_top; }

  inline void set_offset_left(int offset_left) { offset_left_ = offset_left; }

  inline int offset_top() { return offset_top_; }

  inline int offset_left() { return offset_left_; }

  inline int offset_width() { return offset_width_; }

  inline int offset_height() { return offset_height_; }

  friend class CSSStaticLayout;
  virtual base::Size Measure(int width_descriptor, int height_descriptor);

  // Subclasses should override onMeasure(int, int) to provide
  // a measurements of their content.
  virtual base::Size OnMeasure(int width_descriptor, int height_descriptor);

  // if you want to modify postion, override this function
  virtual void Layout(int left, int top, int right, int bottom);

  // Called from layout when this node should assign a position to each of its
  // children. if you want to use new layout mode, override this function
  virtual void OnLayout(int left, int top, int right, int bottom);

 protected:
  enum LAYOUT_STATE {
    LAYOUT_STATE_DIRTY,
    LAYOUT_STATE_UP_TO_DATE,
  };

  void UpToDate();

  bool NeedRemeasure(int width, int height);

  base::Size measured_size_;
  base::Position measured_position_;

  LAYOUT_STATE layout_state_;

  CSSStyle css_style_;

  int offset_top_;
  int offset_left_;
  int offset_width_;
  int offset_height_;

 private:
  base::Size last_measured_size_from_parent_;
};
}  // namespace lynx

#endif  // LYNX_LAYOUT_LAYOUT_OBJECT_H_
