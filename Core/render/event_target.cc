// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/event_target.h"

#include "runtime/base/lynx_array.h"
#include "base/debug/memory_debug.h"

#include "base/trace_event/trace_event_common.h"

namespace lynx {

void EventTarget::DispatchEvent(const std::string& event, base::ScopedPtr<jscore::LynxArray> args) {
    TRACE_EVENT0("renderer", "EventTarget::DispatchEvent");
    EventListenerMap::iterator iter = event_listener_map_.find(event);
    if (iter == event_listener_map_.end()) return;
    size_t length = iter->second->size();
    for (int i = 0; i < length; i++) {
        (*iter->second)[i]->function_->Run(target_data_, args.Get());
    }
}

void EventTarget::AddEventListener(const std::string& event,
                                    jscore::LynxFunction* function,
                                    bool capture) {
    EventListenerMap::iterator iter = event_listener_map_.find(event);

    if (iter == event_listener_map_.end()) {
        iter = event_listener_map_.add(event,
                        lynx_new base::ScopedVector<EventListener>()).first;
        RegisterEvent(event, EVENT_ADD);
    }
    iter->second->push_back(lynx_new EventListener(function, capture));
}

void EventTarget::RemoveEventListener(const std::string& event,
                                      jscore::LynxFunction* function) {
    EventListenerMap::iterator iter = event_listener_map_.find(event);
    if (iter == event_listener_map_.end()) {
        return;
    }

    base::ScopedVector<EventListener>::iterator vec_iter =
                                                    iter->second->begin();

    for (; vec_iter != iter->second->end(); vec_iter++) {
        if ((*vec_iter)->function_->GetKey().compare(function->GetKey()) == 0) {
            iter->second->erase(vec_iter);
            break;
        }
    }
    if(iter->second->empty()) {
        RegisterEvent(event, EVENT_REMOVE);
    }
}

void EventTarget::RemoveEventListener(const std::string& event) {
    EventListenerMap::iterator iter = event_listener_map_.find(event);
    if (iter == event_listener_map_.end()) {
        return;
    }

    event_listener_map_.erase(iter);

    RegisterEvent(event, EVENT_REMOVE);
}
}  // namespace lynx
