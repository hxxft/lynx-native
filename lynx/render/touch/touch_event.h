// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_TOUCH_EVENT_H
#define LYNX_RENDER_TOUCH_EVENT_H

#include "runtime/js_event.h"
#include "touch_event_info.h"

namespace lynx {
    class TouchEvent : public jscore::JSEvent {
    public:

        TouchEvent(TouchEventInfo& info);
        virtual ~TouchEvent();

        static TouchEvent* Obtain(TouchEventInfo info);

        inline std::string touch_event_type() {
            return touch_event_type_;
        }

        inline std::string motion_event_type() {
            return motion_event_type_;
        }

        void Recycle();

        inline float x() {
            return x_;
        }

        inline float y() {
            return y_;
        }

    private:

        static base::ScopedPtr<jscore::LynxValue> GetTouchesCallback(jscore::LynxObjectTemplate* object);
        std::string touch_event_type_;
        std::string motion_event_type_;
        float x_;
        float y_;
    };
}

#endif //LYNX_RENDER_TOUCH_EVENT_H
