// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_EVENT_TARGET_H_
#define LYNX_RENDER_EVENT_TARGET_H_

#include <string>

#include "base/scoped_ptr_map.h"
#include "base/scoped_vector.h"
#include "base/scoped_ptr.h"
#include "runtime/base/lynx_function.h"

namespace lynx {
//typedef jscore::JSFunction EventListener;

static std::string kAnimateEvent = "@LynxAnimate@";

class JSArray;
class EventListener {
public:
    bool capture_;
    base::ScopedPtr<jscore::LynxFunction> function_;

    EventListener(jscore::LynxFunction *function, bool capture) :
            capture_(capture), function_(function){

    }
};

class EventTarget {
 public:
    EventTarget() : target_data_(NULL),
        event_listener_map_() {
    }
    virtual void DispatchEvent(const std::string& event, base::ScopedPtr<jscore::LynxArray> args);

    virtual void AddEventListener(const std::string& event,  jscore::LynxFunction* listener, bool capture);

    virtual void RemoveEventListener(const std::string& event, jscore::LynxFunction* listener);

    void* GetTarget() {
        return target_data_;
    }

    void SetTarget(void* target) {
        target_data_ = target;
    }

 protected:
    enum RegisterEventType {
        EVENT_ADD,
        EVENT_REMOVE,
    };

    virtual void RegisterEvent(const std::string& event,
                                RegisterEventType type) {
    }

protected:
    typedef base::ScopedPtrMap<std::string, base::ScopedVector<EventListener>> EventListenerMap;

    void* target_data_;

    EventListenerMap event_listener_map_;
};
}  // namespace lynx


#endif  // LYNX_RENDER_EVENT_TARGET_H_
