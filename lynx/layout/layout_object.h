// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_LAYOUT_LAYOUT_OBJECT_H_
#define LYNX_LAYOUT_LAYOUT_OBJECT_H_

#include <string>

#include "base/size.h"
#include "base/position.h"
#include "layout/container_node.h"
#include "layout/css_style.h"

namespace lynx {
class LayoutObject : public ContainerNode {
 public:
    LayoutObject();
    virtual ~LayoutObject();

    virtual void InsertChild(ContainerNode* child, int index);
    virtual void RemoveChild(ContainerNode* child);

    virtual base::Size Measure(int width, int height);
    virtual void Layout(int left, int top, int right, int bottom);

    virtual void SetStyle(const std::string& key,
                        const std::string& value) {
        style_.SetValue(key, value);
    }

    CSSStyle* GetStyle() { return &style_; }

    base::Size& GetMeasuredSize() {
        return measured_size_;
    }

    enum LAYOUT_STATE {
        LAYOUT_STATE_DIRTY,
        LAYOUT_STATE_UP_TO_DATE,
    };

    void Dirty();

    void UpToDate();

    bool IsDirty();

    bool ShouldRemeasure(int width, int height);

    base::Size measured_size_;
    base::Position measured_position_;

    LAYOUT_STATE layout_state_;

    CSSStyle style_;

private:
    int last_measured_width_from_parent_;
    int last_measured_height_from_parent_;
};
}  // namespace lynx

#endif  // LYNX_LAYOUT_LAYOUT_OBJECT_H_
