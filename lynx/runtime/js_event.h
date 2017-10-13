// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef ANDROID_JS_EVENT_H
#define ANDROID_JS_EVENT_H

#include <string>
#include "runtime/base/lynx_object_template.h"
#include "runtime/base/lynx_value.h"
#include "render/render_object.h"

namespace jscore {
    class JSEvent : public LynxObjectTemplate {

    public:
        JSEvent(std::string event);
        virtual ~JSEvent();

        void InitEvent(std::string type, bool bubbles, bool cancelable);
        void PreventDefault();
        void StopPropagation();

        void set_type(std::string type) {
            type_ = type;
        }

        std::string type() {
            return type_;
        }

        inline void set_target(lynx::RenderObject* target) {
            target_ = target;
        }

        inline lynx::RenderObject* target() {
            return target_;
        }

        inline void set_current_target(lynx::RenderObject* current_target) {
            current_target_ = current_target;
        }

        inline lynx::RenderObject* current_target() {
            return current_target_;
        }

        inline bool bubbles() {
            return bubbles_;
        }

        inline bool cancel_bubble() {
            return bubbles_;
        }

        inline bool cancelable() {
            return cancelable_;
        }

    private:
        static base::ScopedPtr<LynxValue> GetTypeCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetBubblesCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetCancelableCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetTargetCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetCurrentTargetCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> StopPropagationCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& value);
        static base::ScopedPtr<LynxValue> PreventDefaultCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& value);
        static base::ScopedPtr<LynxValue> InitEventCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& value);
        void Reset();

        lynx::RenderObject* current_target_;
        lynx::RenderObject* target_;

        bool default_prevented_;
        bool cancel_bubble_;
        bool cancelable_;
        bool bubbles_;

        long time_stamp;

        std::string type_;

    };
}


#endif //ANDROID_JS_EVENT_H
