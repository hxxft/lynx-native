// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_LIST_VIEW_H_
#define LYNX_RENDER_LIST_VIEW_H_

#include "render/cell_container.h"
#include "render/render_object.h"

namespace lynx {

class ListView : public RenderObject {
 public:
  ListView(const char* tag_name, uint64_t id, RenderTreeHost* host);
  virtual ~ListView();
  virtual void InsertChild(ContainerNode* child, int index) override;
  virtual void InsertBefore(RenderObject* child,
                            RenderObject* reference) override;
  virtual void RemoveChild(ContainerNode* child) override;

 protected:
  virtual void OnLayout(int left, int top, int right, int bottom) override;

 private:
  CellsContainer container_;
};
}  // namespace lynx

#endif  // LYNX_RENDER_LIST_VIEW_H_
