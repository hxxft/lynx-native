// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/coordinator/coordinator_executor.h"

#include "lepus/vm_context.h"
#include "lepus/builtin.h"

namespace lynx {

    typedef lepus::Value (*NativeFunction)(const std::vector<lepus::Value>& args);

    CoordinatorExecutor::CoordinatorExecutor(const std::string &executable) {

        vm_ = new lepus::VM();
        lepus::VMContext* ctx = new lepus::VMContext();
        ctx->Initialize();

        lepus::RegisterCFunction(ctx, "setTranslateY", action_.SetTranslateY);
        lepus::RegisterCFunction(ctx, "setTranslateX", action_.SetTranslateX);
        lepus::RegisterCFunction(ctx, "setScaleX", action_.SetScaleX);
        lepus::RegisterCFunction(ctx, "setScaleY", action_.SetScaleY);
        lepus::RegisterCFunction(ctx, "setRotateX", action_.SetRotateX);
        lepus::RegisterCFunction(ctx, "setRotateY", action_.SetRotateY);
        lepus::RegisterCFunction(ctx, "setOriginX", action_.SetOriginX);
        lepus::RegisterCFunction(ctx, "setOriginY", action_.SetOriginY);
        lepus::RegisterCFunction(ctx, "setOpacity", action_.SetOpacity);
        lepus::RegisterCFunction(ctx, "setTopOffset", action_.SetTopOffset);
        lepus::RegisterCFunction(ctx, "setBottomOffset", action_.SetBottomOffset);
        lepus::RegisterCFunction(ctx, "setLeftOffset", action_.SetLeftOffset);
        lepus::RegisterCFunction(ctx, "setRightOffset", action_.SetRightOffset);
        lepus::RegisterCFunction(ctx, "setConsumed", action_.SetConsumed);
        lepus::RegisterCFunction(ctx, "setDuration", action_.SetDuration);
        lepus::RegisterCFunction(ctx, "dispatchEvent", action_.DispatchEvent);
        lepus::RegisterCFunction(ctx, "setTimingFunction", action_.SetTimingFunction);

        vm_->Execute(ctx, executable);
        ctx_ = ctx;
    }

    CoordinatorExecutor::~CoordinatorExecutor() {
        lynx_delete(vm_);
        lynx_delete(ctx_);
    }

    CoordinatorAction CoordinatorExecutor::Execute(const std::string& method,
                                                   const std::vector<lepus::Value>& args) {
        action_.Reset();
        vm_->Call(ctx_, method, args);
        return action_;
    }
}