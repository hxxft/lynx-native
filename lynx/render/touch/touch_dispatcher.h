// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_TOUCH_DISPATCHER_H
#define LYNX_RENDER_TOUCH_DISPATCHER_H

#include "render/touch/touch_event_info.h"
#include <vector>

namespace lynx {
    class RenderObject;
    class TouchEvent;
    class TouchDispatcher {
    public:
        TouchDispatcher(RenderObject* render_object);
        ~TouchDispatcher();

        void DispatchTouchEvent(TouchEventInfo info);

    private:
        void CaptureEvent(float axis_x, float axis_y, TouchEvent* touchEvent);

        RenderObject* FindNextTargetConsideringFixedAndZIndex(RenderObject* target,
                                                              float axis_x,
                                                              float axis_y);

        RenderObject* FindNextTargetStraightly(RenderObject* target, float axis_x, float axis_y);

        bool CheckTouchInside(float axis_x, float axis_y, RenderObject* node);

        bool IsUnVisibleNode(RenderObject* node);

        bool IsFixedNode(RenderObject* node);

        void BubbleEvent(TouchEvent* touch_event);

        void HandleTouch(RenderObject* node, TouchEvent* touch_event);

        void HandleMotion(RenderObject* node, TouchEvent* touch_event);

        void Reset();

        RenderObject* root_render_object_;
        std::vector<RenderObject*> render_object_stack_;
    };
}

#endif //LYNX_RENDER_TOUCH_DISPATCHER_H
