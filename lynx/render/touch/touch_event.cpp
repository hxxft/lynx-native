// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/base/lynx_value.h"
#include "render/touch/touch_event.h"

namespace lynx {

    TouchEvent::TouchEvent(TouchEventInfo& info) : JSEvent(info.touch_event_type()),
                                                   touch_event_type_(info.touch_event_type()),
                                                   motion_event_type_(info.motion_event_type()),
                                                   x_(info.GetX()),
                                                   y_(info.GetY()) {
        set_class_name("TouchEvent");
        RegisterAccessorCallback("changedTouches", &GetTouchesCallback, 0);
        RegisterAccessorCallback("touches", &GetTouchesCallback, 0);
    }

    TouchEvent::~TouchEvent() {

    }

    void TouchEvent::Recycle() {

    }

    TouchEvent* TouchEvent::Obtain(TouchEventInfo info) {
        TouchEvent* event = lynx_new TouchEvent(info);
        return event;
    }

    base::ScopedPtr<jscore::LynxValue> TouchEvent::GetTouchesCallback(jscore::LynxObjectTemplate* object) {
        jscore::LynxArray* touches = lynx_new jscore::LynxArray();
        jscore::LynxObject* axis = lynx_new jscore::LynxObject();

        float x = static_cast<TouchEvent*>(object)->x();
        float y = static_cast<TouchEvent*>(object)->y();
        jscore::LynxValue* x_value = jscore::LynxValue::MakeFloat(x);
        jscore::LynxValue* y_value = jscore::LynxValue::MakeFloat(y);
        axis->Set("clientX", x_value);
        axis->Set("clientY", y_value);
        axis->Set("screenX", x_value);
        axis->Set("screenY", y_value);

        touches->Push(axis);
        return jscore::LynxValue::MakeValueScoped(touches);
    }

}