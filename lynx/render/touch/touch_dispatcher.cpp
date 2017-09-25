// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/touch/touch_dispatcher.h"
#include "render/touch/touch_event.h"

namespace lynx {
    TouchDispatcher::TouchDispatcher(RenderObject* render_object) : root_render_object_(render_object) {

    }

    TouchDispatcher::~TouchDispatcher() {

    }

    void TouchDispatcher::DispatchTouchEvent(TouchEventInfo info) {
        // Obtain touch event
        TouchEvent* touch_event = TouchEvent::Obtain(info);

        // Capture
        if (touch_event->touch_event_type().compare(kTouchStartEventName) == 0) {
            CaptureEvent(info.GetX(), info.GetY(), touch_event);
        }

        // Bubble
        BubbleEvent(touch_event);

        // Recycle and reset
        touch_event->Recycle();
        
        if (touch_event->touch_event_type().compare(kTouchCancelEventName) == 0 ||
                touch_event->touch_event_type().compare(kTouchEndEventName) == 0) {
           Reset();
        }
    }

    void TouchDispatcher::CaptureEvent(float axis_x, float axis_y, TouchEvent* touchEvent) {
        RenderObject* target = root_render_object_;

        float offsetX = 0;
        float offsetY = 0;

        while (target != NULL) {

            axis_x = axis_x - offsetX;
            axis_y = axis_y - offsetY;

            // Do not participate in bubble If node has no event listener,
            if (!target->IsEventListenerEmpty()) {
                render_object_stack_.push_back(target);
            }

            // Throw capturing event, maybe some event listener want to response.
            target->OnCapturingTouchEvent(touchEvent);

            RenderObject* next_target = NULL;

            if (target->render_object_type() == LYNX_BODY) {
                // Resort children accroding to z-index and fixed node.
                // Now only support z-index on body and solve fixed node.
                next_target = FindNextTargetConsideringFixedAndZIndex(target, axis_x, axis_y);
            } else {
                next_target = FindNextTargetStraightly(target, axis_x, axis_y);
            }

            if (next_target != NULL) {
                offsetX = next_target->measured_position_.left_ - target->scroll_left();
                offsetY = next_target->measured_position_.top_ - target->scroll_top();
                target = next_target;
            } else {
                // The final target should participate in bubble
                if (target->IsEventListenerEmpty()) {
                    render_object_stack_.push_back(target);
                }
                break;
            }

        }
    }

    // This method will take fixed and z-index style into consideration
    RenderObject *TouchDispatcher::FindNextTargetConsideringFixedAndZIndex(
            RenderObject *target,
            float axis_x,
            float axis_y) {
        int higher_zindex = INT_MIN;
        RenderObject* next_target = NULL;

        // Handle fixed node
        std::vector<RenderObject*> fixedNodes = target->GetFixedNodes();
        for (int i = (int) fixedNodes.size() - 1; i >= 0; --i) {
            // Pass hidden and not display one
            if (IsUnVisibleNode(fixedNodes.at(i))) {
                continue;
            }
            if (CheckTouchInside(axis_x, axis_y, fixedNodes.at(i))
                && fixedNodes.at(i)->style_.zindex_ > higher_zindex) {
                next_target = fixedNodes.at(i);
                higher_zindex = next_target->style_.zindex_;
            }
        }

        // Handle children
        for (int i = target->GetChildCount() - 1; i >= 0; --i) {
            // Pass hidden and not display one
            if (IsFixedNode((RenderObject*) target->Get(i))
                || IsUnVisibleNode((RenderObject*) target->Get(i))) {
                continue;
            }
            if (CheckTouchInside(axis_x, axis_y, (RenderObject*) target->Get(i))
                && target->Get(i)->style_.zindex_ > higher_zindex) {
                next_target = (RenderObject*) target->Get(i);
                higher_zindex = next_target->style_.zindex_;
            }
        }

        return next_target;
    }

    // This method find next target in touch area without considering fixed or z-index
    RenderObject* TouchDispatcher::FindNextTargetStraightly(RenderObject* target,
                                                            float axis_x, float axis_y) {
        RenderObject* next_target = NULL;
        // If node has children, dispatch touch event.
        for (int i = target->GetChildCount() - 1; i >= 0; --i) {

            // Only solve fixed item on body
            if (IsFixedNode((RenderObject*) target->Get(i))
                || IsUnVisibleNode((RenderObject*) target->Get(i))) {
                continue;
            }
            // Check if touch in the region of the node.
            if (CheckTouchInside(axis_x + target->scroll_left(),
                                 axis_y + target->scroll_top(),
                                 (RenderObject*) target->Get(i))) {
                next_target = (RenderObject*) target->Get(i);
                break;
            }
        }
        return next_target;
    }

    bool TouchDispatcher::CheckTouchInside(float axis_x, float axis_y, RenderObject* render_object) {
        return axis_x >= render_object->measured_position_.left_ &&
               axis_x <= render_object->measured_position_.right_ &&
               axis_y >= render_object->measured_position_.top_ &&
               axis_y <= render_object->measured_position_.bottom_;
    }

    bool TouchDispatcher::IsUnVisibleNode(RenderObject* render_object) {
        return render_object->style_.css_display_type_ == CSSStyleType::CSS_DISPLAY_NONE
               || render_object->style_.visible_ == CSSStyleType::CSS_HIDDEN;
    }

    bool TouchDispatcher::IsFixedNode(RenderObject* render_object) {
        return render_object->style_.css_position_type_ == CSSStyleType::CSS_POSITION_FIXED;
    }

    void TouchDispatcher::BubbleEvent(TouchEvent* touch_event) {
        bool hasTargetBeenSet = false;
        RenderObject* current_target;
        for (int i = (int) render_object_stack_.size() - 1; i >= 0; --i) {
            current_target = render_object_stack_.at(i);

            // Set current response target
            if (current_target->GetJSRef() == NULL) {
                continue;
            }

            if (!hasTargetBeenSet) {
                // Set the final response target.
                touch_event->set_target(current_target);
                hasTargetBeenSet = true;
            }

            touch_event->set_current_target(current_target);

            // Handle self touch event
            HandleTouch(current_target, touch_event);
            HandleMotion(current_target, touch_event);

            // If bubble is false, prevent bubbling.
            if (touch_event->cancel_bubble() || touch_event->bubbles()) {
                break;
            }
        }
    }

    void TouchDispatcher::HandleTouch(RenderObject* render_object, TouchEvent* touch_event) {
        touch_event->set_type(touch_event->touch_event_type());
        render_object->PerformTouch(touch_event);
    }

    void TouchDispatcher::HandleMotion(RenderObject* render_object, TouchEvent* touch_event) {
        if (!touch_event->motion_event_type().empty()) {
            touch_event->set_type(touch_event->motion_event_type());
            render_object->PerformMotion(touch_event);
        }
    }

    void TouchDispatcher::Reset() {
        render_object_stack_.clear();
    }
}