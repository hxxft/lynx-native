// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_CELL_VIEW_H_
#define LYNX_RENDER_CELL_VIEW_H_

#include "render/view.h"

namespace lynx {

class CellView : public View {
 public:
  CellView(jscore::ThreadManager* manager, RenderTreeHost* host);
  virtual ~CellView() {}

 protected:
  virtual void OnLayout(int left, int top, int right, int bottom) override;
};
}  // namespace lynx

#endif  // LYNX_RENDER_CELL_VIEW_H_
