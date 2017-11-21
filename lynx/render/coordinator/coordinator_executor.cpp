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

        lepus::RegisterCFunction(ctx, "setTranslateY", (void *) action_.SetTranslateY);
        lepus::RegisterCFunction(ctx, "setTranslateX", (void *) action_.SetTranslateX);
        lepus::RegisterCFunction(ctx, "setScaleX", (void *) action_.SetScaleX);
        lepus::RegisterCFunction(ctx, "setScaleY", (void *) action_.SetScaleY);
        lepus::RegisterCFunction(ctx, "setRotateX", (void *) action_.SetRotateX);
        lepus::RegisterCFunction(ctx, "setRotateY", (void *) action_.SetRotateY);
        lepus::RegisterCFunction(ctx, "setPivotX", (void *) action_.SetPivotX);
        lepus::RegisterCFunction(ctx, "setPivotY", (void *) action_.SetPivotY);
        lepus::RegisterCFunction(ctx, "setAlpha", (void *) action_.SetAlpha);
        lepus::RegisterCFunction(ctx, "setOffsetTop", (void *) action_.SetOffsetTop);
        lepus::RegisterCFunction(ctx, "setOffsetBottom", (void *) action_.SetOffsetBottom);
        lepus::RegisterCFunction(ctx, "setOffsetLeft", (void *) action_.SetOffsetLeft);
        lepus::RegisterCFunction(ctx, "setOffsetRight", (void *) action_.SetOffsetRight);
        lepus::RegisterCFunction(ctx, "setConsumed", (void *) action_.SetConsumed);
        lepus::RegisterCFunction(ctx, "setDuration", (void *) action_.SetDuration);
        lepus::RegisterCFunction(ctx, "dispatchEvent", (void *) action_.DispatchEvent);
        lepus::RegisterCFunction(ctx, "setInterpolatorType", (void *) action_.SetInterpolatorType);

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