// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_OBSERVER_OBSERVER_LIST_H_
#define LYNX_BASE_OBSERVER_OBSERVER_LIST_H_

namespace base {
    class Observer;
    class ObserverList {
    public:
        ObserverList() : head_(nullptr), tail_(nullptr) {}
        ~ObserverList() {}
        
        void AddObserver(Observer* obs);
        void RemoveObserver(Observer* obs);
        void Clear();
        void ForEachObserver();
    private:
        Observer* head_;
        Observer* tail_;
    };
}

#endif  // LYNX_BASE_OBSERVER_OBSERVER_LIST_H_
