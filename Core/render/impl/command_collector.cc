// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/impl/command_collector.h"

#include "base/debug/memory_debug.h"
#include "render/render_object.h"
#include "render/impl/render_object_impl.h"
#include "render/impl/render_command.h"

namespace lynx {

RenderCommandCollector::RenderCommandCollector() {
    commands_ = lynx_new base::ScopedVector<RenderCommand>();
}

RenderCommandCollector::~RenderCommandCollector() {
    lynx_delete(commands_);
}

void RenderCommandCollector::Collect(RenderCommand* command) {
    commands_->push_back(command);
}

void RenderCommandCollector::Push() {
    base::AutoLock lock(lock_);
    collectors_.push(commands_);
    commands_ = lynx_new base::ScopedVector<RenderCommand>();
}

base::ScopedVector<RenderCommand>* RenderCommandCollector::Pop() {
    base::AutoLock lock(lock_);

    RenderCommands* collector = NULL;
    if (!collectors_.empty()) {
        collector = collectors_.front();
        collectors_.pop();
    }
    return collector;
}
}  // namespace lynx
