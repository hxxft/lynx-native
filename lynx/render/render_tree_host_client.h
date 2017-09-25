// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_RENDER_TREE_HOST_CLIENT_H_
#define LYNX_RENDER_RENDER_TREE_HOST_CLIENT_H_

namespace lynx {
class RenderTreeHostClient {
 public:
    virtual void PrepareCommit() = 0;
};
}

#endif  // LYNX_RENDER_RENDER_TREE_HOST_CLIENT_H_
