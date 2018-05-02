// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_LIST_SHADOW_H_
#define LYNX_RENDER_LIST_SHADOW_H_

#include "render/cell_container.h"
#include "render/view.h"

namespace lynx {
class ListShadow : public View {
 public:
  ListShadow(const char* tag_name, uint64_t id, RenderTreeHost* host);
  virtual ~ListShadow() {}
  virtual void InsertChild(ContainerNode* child, int index) override;
  virtual void RemoveChild(ContainerNode* child) override;
  virtual void InsertBefore(RenderObject* child,
                            RenderObject* reference) override;

 protected:
  virtual void OnLayout(int left, int top, int right, int bottom) override;

 private:
  CellsContainer container_;
};
}  // namespace lynx

#endif  // LYNX_RENDER_LIST_SHADOW_H_
