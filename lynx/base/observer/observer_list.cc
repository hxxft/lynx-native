// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/observer/observer_list.h"

#include "base/observer/observer.h"
#include "base/debug/memory_debug.h"

namespace base {
    ObserverList::~ObserverList() {
        if(head_ == tail_)
            return;
        Observer* ptr = head_;
        do{
            Observer* next = ptr->next_;
            lynx_delete(ptr);
            ptr = next;
        }while(ptr != tail_);
    }
    
    void ObserverList::AddObserver(Observer *obs) {
        if (head_ == nullptr) {
            head_ = obs;
            tail_ = obs;
            obs->previous_ = head_;
            obs->next_ = tail_;
        } else {
            tail_->next_ = obs;
            obs->previous_ = tail_;
            obs->next_ = tail_->next_;
            tail_ = obs;
        }
    }
    
    void ObserverList::RemoveObserver(Observer *obs) {
        if(obs == head_) {
            head_ = obs->next_;
            obs->next_->previous_ = head_;
        }else if(obs == tail_) {
            tail_ = obs->previous_;
            obs->previous_->next_ = tail_;
        }else {
            obs->previous_->next_ = obs->next_;
            obs->next_->previous_ = obs->previous_;
        }

    }
    
    void ObserverList::ForEachObserver() {
        if(head_ == tail_)
            return;
        Observer* ptr = head_;
        do{
            Observer* next = ptr->next_;
            ptr->Update();
            ptr = next;
        }while(ptr != tail_);
    }
}
