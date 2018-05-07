// Copyright 2017 The Lynx Authors. All rights reserved.
#ifndef LYNX_RUNTIME_IOS_LYNX_OBJECT_IOS_H_
#define LYNX_RUNTIME_IOS_LYNX_OBJECT_IOS_H_

#include "base/lynx_object.h"
#include "runtime/base/lynx_object_platform.h"

namespace jscore {
    class LynxObjectIOS : public LynxObjectPlatform {
    public:
        LynxObjectIOS(LxObject *ios_object);
        virtual ~LynxObjectIOS();
        
        base::ScopedPtr<LynxValue> Exec(const std::string& name,
                                        base::ScopedPtr<LynxArray> array);
        
    private:
        LxObject *ios_object_;
    };
}


#endif //LYNX_RUNTIME_IOS_LYNX_OBJECT_IOS_H_
