// Copyright 2017 The Lynx Authors. All rights reserved.

#include <runtime/base/lynx_value.h>
#include "runtime/location.h"
#include "runtime/base/lynx_array.h"

namespace jscore {
    
    Location::Location(JSContext* context) : context_(context) {
        set_class_name("Location");
        RegisterMethodCallback("replace", &ReplaceCallback);
        RegisterMethodCallback("reload", &ReloadCallback);
        RegisterMethodCallback("assign", &AssignCallback);
        RegisterAccessorCallback("hash", &GetHashCallback, &SetHashCallback);
        RegisterAccessorCallback("host", &GetHostCallback, &SetHostCallback);
        RegisterAccessorCallback("hostname", &GetHostNameCallback, &SetHostNameCallback);
        RegisterAccessorCallback("href", &GetHrefCallback, &SetHrefCallback);
        RegisterAccessorCallback("pathname", &GetPathNameCallback, &SetPathNameCallback);
        RegisterAccessorCallback("port", &GetPortCallback, &SetPortCallback);
        RegisterAccessorCallback("protocol", &GetProtocolCallback, &SetProtocolCallback);
        RegisterAccessorCallback("search", &GetSearchCallback, &SetSearchCallback);
        RegisterAccessorCallback("origin", &GetOriginCallback, &SetOriginCallback);
    }
    
    Location::~Location() {
        
    }
    
    void Location::SetUrl(const std::string& url) {
        ParseHref(url);
    }
    
    void Location::ParseHref(const std::string& href) {
        uri_ = net::Uri::Parse(href);
        href_ = uri_.href_;
        host_ = uri_.host_;
        protocol_ = uri_.protocol_;
        port_ = uri_.port_;
        pathname_ = uri_.path_;
        search_ = uri_.query_;
        origin_ = uri_.origin_;
        hostname_ = uri_.hostname_;
    }
    
    void Location::Reload(bool force) {
//        context_->Reload(force);
    }
    
    void Location::Replace(const std::string &url) {
        SetUrl(url);
//        context_->Reload(true);
    }
    
    void Location::Assign(const std::string &url) {
        context_->LoadUrl(url);
    }

    base::ScopedPtr<LynxValue>
    Location::ReplaceCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array) {
        Location* location = static_cast<Location*>(object);
        if (array.Get() != NULL && array->Size() > 0) {
            LynxValue* value = array->Get(0);
            if (value->type_ == LynxValue::Type::VALUE_STRING) {
                std::string url = value->data_.str;
                location->Replace(url);
            }
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }


    base::ScopedPtr<LynxValue>
    Location::ReloadCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array) {
        Location* location = static_cast<Location*>(object);
        if (array.Get() != NULL && array->Size() > 0) {
            LynxValue* value = array->Get(0);
            if (value->type_ == LynxValue::Type::VALUE_BOOL) {
                location->Reload(value->data_.b);
            }
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Location::AssignCallback(LynxObjectTemplate *object, base::ScopedPtr<LynxArray>& array) {
        Location* location = static_cast<Location*>(object);
        if (array.Get() != NULL && array->Size() > 0) {
            LynxValue* value = array->Get(0);
            if (value->type_ == LynxValue::Type::VALUE_STRING) {
                std::string url = value->data_.str;
                location->Assign(url);
            }
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Location::GetHashCallback(LynxObjectTemplate* object) {
        Location* location = static_cast<Location*>(object);
        LynxValue *value = LynxValue::MakeString(location->hash());
        return base::ScopedPtr<LynxValue>(value);
    }

    base::ScopedPtr<LynxValue> Location::GetHostCallback(LynxObjectTemplate* object) {
        Location* location = static_cast<Location*>(object);
        LynxValue *value = LynxValue::MakeString(location->host());
        return base::ScopedPtr<LynxValue>(value);
    }

    base::ScopedPtr<LynxValue> Location::GetHostNameCallback(LynxObjectTemplate* object) {
        Location* location = static_cast<Location*>(object);
        LynxValue *value = LynxValue::MakeString(location->hostname());
        return base::ScopedPtr<LynxValue>(value);
    }

    base::ScopedPtr<LynxValue> Location::GetHrefCallback(LynxObjectTemplate* object) {
        Location* location = static_cast<Location*>(object);
        LynxValue *value = LynxValue::MakeString(location->href());
        return base::ScopedPtr<LynxValue>(value);
    }

    base::ScopedPtr<LynxValue> Location::GetPathNameCallback(LynxObjectTemplate* object) {
        Location* location = static_cast<Location*>(object);
        LynxValue *value = LynxValue::MakeString(location->pathname());
        return base::ScopedPtr<LynxValue>(value);
    }

    base::ScopedPtr<LynxValue> Location::GetPortCallback(LynxObjectTemplate* object) {
        Location* location = static_cast<Location*>(object);
        LynxValue *value = LynxValue::MakeString(location->port());
        return base::ScopedPtr<LynxValue>(value);
    }

    base::ScopedPtr<LynxValue> Location::GetProtocolCallback(LynxObjectTemplate* object) {
        Location* location = static_cast<Location*>(object);
        LynxValue *value = LynxValue::MakeString(location->protocol());
        return base::ScopedPtr<LynxValue>(value);
    }

    base::ScopedPtr<LynxValue> Location::GetSearchCallback(LynxObjectTemplate* object) {
        Location* location = static_cast<Location*>(object);
        LynxValue *value = LynxValue::MakeString(location->search());
        return base::ScopedPtr<LynxValue>(value);
    }

    base::ScopedPtr<LynxValue> Location::GetOriginCallback(LynxObjectTemplate* object) {
        Location* location = static_cast<Location*>(object);
        LynxValue *value = LynxValue::MakeString(location->origin());
        return base::ScopedPtr<LynxValue>(value);
    }

    void Location::SetHashCallback(LynxObjectTemplate* object,
                                base::ScopedPtr<jscore::LynxValue> value) {
        Location* location = static_cast<Location*>(object);
        if (value->type_ == LynxValue::Type::VALUE_STRING) {
            location->set_hash(value->data_.str);
        }
    }

    void Location::SetHostCallback(LynxObjectTemplate* object,
                                base::ScopedPtr<jscore::LynxValue> value) {
        Location* location = static_cast<Location*>(object);
        if (value->type_ == LynxValue::Type::VALUE_STRING) {
            location->set_host(value->data_.str);
        }
    }

    void Location::SetHostNameCallback(LynxObjectTemplate* object,
                                    base::ScopedPtr<jscore::LynxValue> value) {
        Location* location = static_cast<Location*>(object);
        if (value->type_ == LynxValue::Type::VALUE_STRING) {
            location->set_hostname(value->data_.str);
        }
    }

    void Location::SetHrefCallback(LynxObjectTemplate* object,
                                base::ScopedPtr<jscore::LynxValue> value) {
        Location* location = static_cast<Location*>(object);
        if (value->type_ == LynxValue::Type::VALUE_STRING) {
            location->set_href(value->data_.str);
        }
    }

    void Location::SetPathNameCallback(LynxObjectTemplate* object,
                                    base::ScopedPtr<jscore::LynxValue> value) {
        Location* location = static_cast<Location*>(object);
        if (value->type_ == LynxValue::Type::VALUE_STRING) {
            location->set_pathname(value->data_.str);
        }
    }

    void Location::SetPortCallback(LynxObjectTemplate* object,
                                base::ScopedPtr<jscore::LynxValue> value) {
        Location* location = static_cast<Location*>(object);
        if (value->type_ == LynxValue::Type::VALUE_STRING) {
            location->set_port(value->data_.str);
        }
    }

    void Location::SetProtocolCallback(LynxObjectTemplate* object,
                                    base::ScopedPtr<jscore::LynxValue> value) {
        Location* location = static_cast<Location*>(object);
        if (value->type_ == LynxValue::Type::VALUE_STRING) {
            location->set_protocol(value->data_.str);
        }
    }

    void Location::SetSearchCallback(LynxObjectTemplate* object,
                                  base::ScopedPtr<jscore::LynxValue> value) {
        Location* location = static_cast<Location*>(object);
        if (value->type_ == LynxValue::Type::VALUE_STRING) {
            location->set_search(value->data_.str);
        }
    }

    void Location::SetOriginCallback(LynxObjectTemplate* object,
                                  base::ScopedPtr<jscore::LynxValue> value) {
        Location* location = static_cast<Location*>(object);
        if (value->type_ == LynxValue::Type::VALUE_STRING) {
            location->set_origin(value->data_.str);
        }
    }
}
