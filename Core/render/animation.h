// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_ANIMATION_H_
#define LYNX_RENDER_ANIMATION_H_

#include "base/weak_ptr.h"
#include "runtime/base/lynx_object_template.h"

namespace lynx {
    class RenderObject;

    class Animation : public jscore::LynxObjectTemplate {
    public:
        Animation(const std::string& id);
        virtual ~Animation();

        void set_render_object(base::WeakPtr<RenderObject>& weak_ptr) {
            render_object_ = weak_ptr;
        }

        static base::ScopedPtr<jscore::LynxValue>
        AddEventListener(jscore::LynxObjectTemplate *object,
                         base::ScopedPtr<jscore::LynxArray>& array);

        static base::ScopedPtr<jscore::LynxValue>
        RemoveEventListener(jscore::LynxObjectTemplate *object,
                            base::ScopedPtr<jscore::LynxArray>& array);

        static base::ScopedPtr<jscore::LynxValue> Finish(jscore::LynxObjectTemplate *object,
                                                         base::ScopedPtr<jscore::LynxArray>& array);

        static base::ScopedPtr<jscore::LynxValue> Cancel(jscore::LynxObjectTemplate *object,
                                                         base::ScopedPtr<jscore::LynxArray>& array);

        static base::ScopedPtr<jscore::LynxValue> Pause(jscore::LynxObjectTemplate *object,
                                                        base::ScopedPtr<jscore::LynxArray>& array);

        static base::ScopedPtr<jscore::LynxValue> Play(jscore::LynxObjectTemplate *object,
                                                       base::ScopedPtr<jscore::LynxArray>& array);

    private:

        void RemoveAllEventListener();

        base::WeakPtr<RenderObject> render_object_;
        std::string id_;
    };
}


#endif //LYNX_RENDER_ANIMATION_H_
