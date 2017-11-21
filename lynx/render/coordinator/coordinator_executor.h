// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_COORDINATOR_EXECUTOR_H_
#define LYNX_RENDER_COORDINATOR_EXECUTOR_H_

#include <string>
#include "lepus/vm.h"
#include "render/coordinator/coordinator_action.h"

namespace lynx {
    class CoordinatorExecutor {
    public:
        CoordinatorExecutor(const std::string& executable);
        ~CoordinatorExecutor();

        CoordinatorAction Execute(const std::string& method, const std::vector<lepus::Value>& args);

    private:
        lepus::Context *ctx_;
        lepus::VM *vm_;

        CoordinatorAction action_;
    };
}

#endif //LYNX_RENDER_COORDINATOR_EXECUTOR_H_
