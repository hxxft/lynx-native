// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_ANIMATION_H_
#define LYNX_RENDER_ANIMATION_H_

#include "base/weak_ptr.h"
#include "render/render_object.h"
#include "runtime/base/lynx_object.h"

namespace jscore {

    class Animation : public jscore::LynxObject {
    public:
        Animation(JSContext *context, const std::string& id);
        virtual ~Animation();

        void set_render_object(base::WeakPtr<lynx::RenderObject>& weak_ptr) {
            render_object_ = weak_ptr;
        }

        base::ScopedPtr<jscore::LynxValue> AddEventListener(base::ScopedPtr<jscore::LynxArray>& array);

        base::ScopedPtr<jscore::LynxValue> RemoveEventListener(base::ScopedPtr<jscore::LynxArray>& array);

        base::ScopedPtr<jscore::LynxValue> Finish(base::ScopedPtr<jscore::LynxArray>& array);

        base::ScopedPtr<jscore::LynxValue> Cancel(base::ScopedPtr<jscore::LynxArray>& array);

        base::ScopedPtr<jscore::LynxValue> Pause(base::ScopedPtr<jscore::LynxArray>& array);

        base::ScopedPtr<jscore::LynxValue> Play(base::ScopedPtr<jscore::LynxArray>& array);

    private:

        void RemoveAllEventListener();

        base::WeakPtr<lynx::RenderObject> render_object_;
        std::string id_;
    };
}


#endif //LYNX_RENDER_ANIMATION_H_
