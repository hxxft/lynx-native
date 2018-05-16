#ifndef LYNX_LEPUS_UPVALUE_H_
#define LYNX_LEPUS_UPVALUE_H_

#include "lepus/lepus_string.h"
#include "base/ref_counted_ptr.h"

namespace lepus {
    struct UpvalueInfo {
        String* name_;
        long register_;
        bool in_parent_vars_;
        
        UpvalueInfo(String* name, long register_id, bool in_parent_vars)
        : name_(name),
        register_(register_id),
        in_parent_vars_(in_parent_vars){
            //name_->AddRef();
        }
        
        ~UpvalueInfo() {
            //name_->Release();
        }
    };
}

#endif
