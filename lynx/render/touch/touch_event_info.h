// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_TOUCH_EVENT_INFO_H
#define LYNX_RENDER_TOUCH_EVENT_INFO_H

#include "runtime/jsc/jsc_helper.h"
#include <assert.h>
#include "runtime/base/lynx_value.h"

namespace lynx {

    static const int kTouchEventNameSize = 5;
    static const std::string kTouchStartEventName = "touchstart";
    static const std::string kTouchMoveEventName = "touchmove";
    static const std::string kTouchEndEventName = "touchend";
    static const std::string kTouchCancelEventName = "touchcancle";
    static const std::string kTouchEventName[] = {kTouchStartEventName,
        kTouchMoveEventName, kTouchEndEventName, kTouchCancelEventName, ""};
    static const int kMotionEventNameSize = 2;
    static const std::string kMotionClickEventName = "click";
    static const std::string kMotionEventName[] = {kMotionClickEventName, ""};

    struct TouchAxis {
        int x_;
        int y_;

        TouchAxis(int x, int y) : x_(x), y_(y) {}
    };

    class TouchEventInfo {
    public:

        TouchEventInfo(int x, int y, long time_stamp,
                       std::string& touch_event_type,
                       std::string& motion_event_type) :
                touch_axis_(x, y),
                touch_event_type_(touch_event_type),
                motion_event_type_(motion_event_type) {

        }

        inline int GetX() {
            return touch_axis_.x_;
        }

        inline int GetY() {
            return touch_axis_.y_;
        }

        inline std::string& touch_event_type() {
            return touch_event_type_;
        }

        inline std::string& motion_event_type() {
            return motion_event_type_;
        }

        static bool IsTouchEvent(std::string event_name) {
            for (int i = 0; i < kTouchEventNameSize; ++i) {
                if (event_name.compare(kTouchEventName[i]) == 0) {
                    return true;
                }
            }
            return false;
        }

        static bool IsMotionEvent(std::string event_name) {
            for (int i = 0; i < kMotionEventNameSize; ++i) {
                if (event_name.compare(kMotionEventName[i]) == 0) {
                    return true;
                }
            }
            return false;
        }

        static TouchEventInfo MakeInfo(jscore::LynxArray* args) {
            assert(args->Size() == 1);
            assert(args->Get(0)->type_ == jscore::LynxValue::Type::VALUE_LYNX_ARRAY);
            jscore::LynxArray* touch_info = args->Get(0)->data_.lynx_array;
            assert(touch_info->Get(0)->type_ == jscore::LynxValue::Type::VALUE_FLOAT);
            assert(touch_info->Get(1)->type_ == jscore::LynxValue::Type::VALUE_FLOAT);
            assert(touch_info->Get(2)->type_ == jscore::LynxValue::Type::VALUE_LONG);
            assert(touch_info->Get(3)->type_ == jscore::LynxValue::Type::VALUE_STRING);
            assert(touch_info->Get(4)->type_ == jscore::LynxValue::Type::VALUE_STRING);
            float x = touch_info->Get(0)->data_.f;
            float y = touch_info->Get(1)->data_.f;
            long time_stamp = touch_info->Get(2)->data_.l;
            std::string touch_event_type = touch_info->Get(3)->data_.str;
            std::string motion_event_type = touch_info->Get(4)->data_.str;
            return TouchEventInfo(x, y, time_stamp, touch_event_type, motion_event_type);
        }

    private:
        TouchAxis touch_axis_;
        std::string touch_event_type_;
        std::string motion_event_type_;
    };
}


#endif //LYNX_RENDER_TOUCH_EVENT_INFO_H
