// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_BODY_H_
#define LYNX_RENDER_BODY_H_

#include "render/view.h"

namespace lynx {

class Body : public View {
 public:
    Body(jscore::ThreadManager* manager, RenderTreeHost* host);
    virtual ~Body() {}

    virtual void Layout(int left, int top, int right, int bottom);
    void Layout();
    void LayoutWithTick(int tick);
    base::Size& body_size() {
        return body_size_;
    }
 private:
    jscore::ThreadManager* thread_manager_;
    base::Size body_size_;
    bool did_first_layout_;
    base::WeakPtr<Body> weak_ptr_;
};
}  // namespace lynx

#endif  // LYNX_RENDER_BODY_H_
