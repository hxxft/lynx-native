// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_COORDINATOR_ACTION_H_
#define LYNX_RENDER_COORDINATOR_ACTION_H_

#include <vector>
#include "lepus/context.h"
#include "lepus/value.h"

namespace lynx {

    class CoordinatorAction {
    public:

#define SetCoordinatorNumber(name, value) \
    static lepus::Value name(lepus::Context* context) { \
        int params_count = context->GetParamsSize(); \
        for(int i = 0; i < params_count; i++) { \
            lepus::Value* v = context->GetParam(i);  \
            switch (v->type_) { \
                case lepus::Value_Number: \
                        value = v->number_; \
                break; \
                default: break; \
            } \
        } \
        return lepus::Value(); \
    } \

#define SetCoordinatorBool(name, value) \
    static lepus::Value name(lepus::Context* context) { \
        int params_count = context->GetParamsSize(); \
        for(int i = 0; i < params_count; i++) { \
            lepus::Value* v = context->GetParam(i);  \
            switch (v->type_) { \
                case lepus::Value_Boolean: \
                        value = v->boolean_; \
                break; \
                default: break; \
            } \
        } \
        return lepus::Value(); \
    } \

        SetCoordinatorNumber(SetTranslateY, translate_y_)
        SetCoordinatorNumber(SetTranslateX, translate_x_)
        SetCoordinatorNumber(SetScaleX, scale_x_)
        SetCoordinatorNumber(SetScaleY, scale_y_)
        SetCoordinatorNumber(SetPivotX, pivot_x_)
        SetCoordinatorNumber(SetPivotY, pivot_y_)
        SetCoordinatorNumber(SetRotateX, rotate_x_)
        SetCoordinatorNumber(SetRotateY, rotate_y_)
        SetCoordinatorNumber(SetAlpha, alpha_)
        SetCoordinatorNumber(SetOffsetTop, offset_top_)
        SetCoordinatorNumber(SetOffsetBottom, offset_bottom_)
        SetCoordinatorNumber(SetOffsetRight, offset_right_)
        SetCoordinatorNumber(SetOffsetLeft, offset_left_)
        SetCoordinatorNumber(SetDuration, duration_)
        SetCoordinatorBool(SetConsumed, consumed_)

        static lepus::Value DispatchEvent(lepus::Context* context) {
            int params_count = context->GetParamsSize();
            if (params_count > 0) {
                event_ = context->GetParam(0)->str_->c_str();
            }
            if (params_count > 1) {
                params_for_event_ = *context->GetParam(1);
            }
            return lepus::Value();
        }

        static void Reset();

        static double translate_x_;
        static double translate_y_;
        static double scale_x_;
        static double scale_y_;
        static double rotate_x_;
        static double rotate_y_;
        static double pivot_x_;
        static double pivot_y_;
        static double alpha_;
        static double offset_top_;
        static double offset_bottom_;
        static double offset_left_;
        static double offset_right_;
        static bool consumed_;
        static double duration_;

        static std::string event_;
        static lepus::Value params_for_event_;

        const static int kNotSet = INT_MAX;
    };
}

#endif //LYNX_RENDER_COORDINATOR_ACTION_H_
