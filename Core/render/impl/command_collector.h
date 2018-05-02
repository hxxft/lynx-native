// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_IMPL_COMMAND_COLLECTOR_H_
#define LYNX_RENDER_IMPL_COMMAND_COLLECTOR_H_

#include <queue>
#include "base/threading/lock.h"
#include "base/scoped_vector.h"
#include "base/scoped_ptr.h"

namespace lynx {
class RenderCommand;
class RenderCommandCollector {
 public:
    typedef base::ScopedVector<RenderCommand> RenderCommands;
    typedef base::ScopedVector<RenderCommand> RenderCommandsStorage;
    RenderCommandCollector();
    ~RenderCommandCollector();

    void Collect(RenderCommand* command);
    void Push();
    RenderCommands* Pop();
 private:
    RenderCommandsStorage* commands_;
    std::queue<RenderCommandsStorage*> collectors_;
    base::Lock lock_;
};
}  // namespace lynx

#endif  // LYNX_RENDER_IMPL_COMMAND_COLLECTOR_H_
