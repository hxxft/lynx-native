// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/history.h"
#include "runtime/base/lynx_value.h"
#include "runtime/base/lynx_array.h"

namespace jscore {
    
    History::History(JSContext* context) : context_(context), pos_(kBlankPos) {
        set_class_name("History");
        RegisterMethodCallback("forward", &ForwardCallback);
        RegisterMethodCallback("back", &BackCallback);
        RegisterMethodCallback("go", &GoCallback);
        RegisterAccessorCallback("length", &GetLengthCallback, 0);
    }
    
    History::~History() {
        
    }
    
    void History::Load() {
        if (pos_ == kBlankPos) {
            context_->LoadUrl("");
        } else {
            const std::string url = history_list_.at(pos_);
            context_->LoadUrl(url);
        }
    }
    
    void History::Forward() {
        Go(1);
    }
    
    void History::Back() {
        Go(-1);
    }
    
    void History::ReloadUrl(const std::string& url) {
        if (pos_ == kBlankPos) {
            return;
        }
        history_list_[pos_] = url;
    }
    
    void History::Go(const std::string& url) {
        int index = kBlankPos;
        for (int i = 0; i < history_list_.size(); ++i) {
            if (url.compare(history_list_.at(i)) == 0) {
                index = i;
            }
        }
        if (index > kBlankPos) {
            pos_ = index;
        } else {
            history_list_.push_back(url);
            pos_++;
        }
    }
    
    void History::Go(int offset) {
        int target = pos_ + offset;
        if (target < kBlankPos || target >= history_list_.size()) {
            return;
        }
        pos_ = target;
    }
    
    int History::GetLength() {
        return (int) history_list_.size();
    }

    base::ScopedPtr<LynxValue>
    History::ForwardCallback(LynxObjectTemplate* object,
                             base::ScopedPtr<LynxArray>& array) {
        History* history = static_cast<History*>(object);
        history->Forward();
        history->Load();
        return LynxValue::MakeValueScoped(NULL);
    }

    base::ScopedPtr<LynxValue>
    History::BackCallback(LynxObjectTemplate* object,
                             base::ScopedPtr<LynxArray>& array) {
        History* history = static_cast<History*>(object);
        history->Back();
        history->Load();
        return LynxValue::MakeValueScoped(NULL);
    }

    base::ScopedPtr<LynxValue>
    History::GoCallback(LynxObjectTemplate* object,
                             base::ScopedPtr<LynxArray>& array) {
        History* history = static_cast<History*>(object);
        if (array.Get() != NULL && array->Size() > 0) {
            LynxValue* value = array->Get(0);
            if (value->type_ == LynxValue::Type::VALUE_INT) {
                int offset = value->data_.i;
                history->Go(offset);
            } else if (value->type_ == LynxValue::Type::VALUE_STRING) {
                std::string url = value->data_.str;
                history->Go(url);
            }
            history->Load();
        }
        return LynxValue::MakeValueScoped(NULL);
    }

    base::ScopedPtr<LynxValue>
    History::GetLengthCallback(LynxObjectTemplate* object) {
        History* history = static_cast<History*>(object);
        LynxValue* length = LynxValue::MakeInt(history->GetLength());
        return LynxValue::MakeValueScoped(length);
    }
    
}
