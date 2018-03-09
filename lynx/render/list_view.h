// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_LIST_VIEW_H_
#define LYNX_RENDER_LIST_VIEW_H_

#include "render/render_object.h"
#include "render/cell_container.h"

namespace lynx {

class ListView : public RenderObject {
 public:
    ListView(const char* tag_name,
             uint64_t id,
             RenderTreeHost* host);
    virtual ~ListView();
    virtual base::Size OnMeasure(int width_descriptor, int height_descriptor);
    virtual void OnLayout(int left, int top, int right, int bottom);
    virtual void InsertChild(ContainerNode* child, int index);
    virtual void InsertBefore(RenderObject* child, RenderObject* reference);
    virtual void RemoveChild(ContainerNode* child);

private:
    CellsContainer container_;
};
}  // namespace lynx

#endif  // LYNX_RENDER_LIST_VIEW_H_
