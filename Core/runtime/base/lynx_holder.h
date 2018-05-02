// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_LYNX_HOLDER_H_
#define LYNX_RUNTIME_LYNX_HOLDER_H_

namespace jscore {

    class LynxHolder {
    public:
        LynxHolder() {
        }

        virtual ~LynxHolder() {}

        virtual base::ScopedPtr<LynxValue> GetLynxValue(){
            return base::ScopedPtr<LynxValue>();
        }

    };

}


#endif //LYNX_RUNTIME_LYNX_HOLDER_H_
