// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/base/lynx_object.h"

#include "runtime/base/lynx_map.h"
#include "runtime/base/lynx_value.h"
#include "runtime/js/class_template.h"
#include "runtime/js/js_context.h"
#include "base/log/logging.h"

namespace jscore {

    LynxObject::LynxObject(JSContext* context, ClassTemplate *class_template)
            : context_(context),
              object_wrap_(NULL),
              class_template_(class_template),
              methods_(),
              protect_times_(0) {

    }

    LynxObject::~LynxObject() {

    }

    void LynxObject::RegisterMethodCallback(const std::string& method_name,
                                            JSMethodCallback callback) {
        if (object_wrap_ != NULL) {
            object_wrap_->RegisterMethodCallback(method_name, callback);
        } else {
            methods_[method_name] = callback;
        }
    }

    void LynxObject::SetObjectWrap(ObjectWrap* object_wrap) {
        object_wrap_ = object_wrap;
        while (protect_times_ > 0) {
            object_wrap_->Protect();
            protect_times_--;
        }
        // Register dynamic function
        for (auto it = methods_.begin(); it != methods_.end(); ++it) {
            object_wrap_->RegisterMethodCallback(it->first, it->second);
        }
    }
    void LynxObject::ProtectJSObject() {
        if (object_wrap_ != NULL) {
            object_wrap_->Protect();
        } else {
            protect_times_++;
        }
    }

    void LynxObject::UnprotectJSObject() {
        if (object_wrap_ != NULL) {
            object_wrap_->Unprotect();
        } else {
            protect_times_--;
        }
    }

}