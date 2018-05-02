// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_OBSERVER_OBSERVER_H_
#define LYNX_BASE_OBSERVER_OBSERVER_H_

namespace base {
    class Observer {
    public:
        Observer() : previous_(nullptr), next_(nullptr) {}
        virtual ~Observer() {}
        virtual void Update() = 0;
        friend class ObserverList;
    private:
        Observer* previous_;
        Observer* next_;
    };
}

#endif  // LYNX_BASE_OBSERVER_OBSERVER_H_
