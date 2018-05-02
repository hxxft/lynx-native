
#include "render/animation.h"
#include "render/render_object.h"
#include "runtime/base/lynx_array.h"
#include "runtime/jsc/jsc_helper.h"
#include "runtime/base/lynx_value.h"

namespace lynx {
    const static std::string kEventFinish = "finish";
    const static std::string kEventCancel = "cancel";

    Animation::Animation(const std::string& id) : render_object_(NULL), id_(id) {
        set_class_name("Animation");

        RegisterMethodCallback("addEventListener", &AddEventListener);
        RegisterMethodCallback("removeEventListener", &RemoveEventListener);
        RegisterMethodCallback("play", &Play);
        RegisterMethodCallback("pause", &Pause);
        RegisterMethodCallback("cancel", &Cancel);
        RegisterMethodCallback("finish", &Finish);
    }

    Animation::~Animation() {
        RemoveAllEventListener();
    }

    base::ScopedPtr<jscore::LynxValue>
    Animation::AddEventListener(jscore::LynxObjectTemplate* object,
                                 base::ScopedPtr<jscore::LynxArray>& array) {
        Animation* animation = (Animation *) object;
        if (array.Get() != NULL && array->Size() == 2
                && array->Get(0)->type_ == jscore::LynxValue::Type::VALUE_STRING
                && array->Get(1)->type_ == jscore::LynxValue::Type::VALUE_LYNX_FUNCTION
                && animation->render_object_.IsValid()) {
            std::string event_name = jscore::JSCHelper::ConvertToString(array->Get(0));
            if (event_name.compare(kEventFinish) == 0 || event_name.compare(kEventCancel) == 0) {
                animation->render_object_->AddEventListener(event_name + animation->id_,
                                                            array->Get(1)->data_.lynx_function,
                                                            false);
            }
        }
        return base::ScopedPtr<jscore::LynxValue>();
    }
    
    base::ScopedPtr<jscore::LynxValue>
    Animation::RemoveEventListener(jscore::LynxObjectTemplate *object,
                                    base::ScopedPtr<jscore::LynxArray>& array) {
        Animation* animation = (Animation *) object;
        if (array.Get() != NULL && array->Size() == 2
                && array->Get(0)->type_ == jscore::LynxValue::Type::VALUE_STRING
                && array->Get(1)->type_ == jscore::LynxValue::Type::VALUE_LYNX_FUNCTION
                && animation->render_object_.IsValid()) {
            std::string event_name = jscore::JSCHelper::ConvertToString(array->Get(0));
            if (event_name.compare(kEventFinish) == 0 || event_name.compare(kEventCancel) == 0) {
                animation->render_object_->RemoveEventListener(event_name + animation->id_,
                                                               array->Get(1)->data_.lynx_function);
            }
        }
        return base::ScopedPtr<jscore::LynxValue>();
    }

    base::ScopedPtr<jscore::LynxValue>
    Animation::Finish(jscore::LynxObjectTemplate *object,
                           base::ScopedPtr<jscore::LynxArray>& array) {

        return base::ScopedPtr<jscore::LynxValue>();
    }

    base::ScopedPtr<jscore::LynxValue>
    Animation::Cancel(jscore::LynxObjectTemplate *object,
                           base::ScopedPtr<jscore::LynxArray>& array) {
        Animation* animation = (Animation *) object;
        if (animation->render_object_.IsValid()) {
            animation->render_object_->CancelAnimation(animation->id_);
        }
        animation->RemoveAllEventListener();
        return base::ScopedPtr<jscore::LynxValue>();
    }

    base::ScopedPtr<jscore::LynxValue>
    Animation::Pause(jscore::LynxObjectTemplate *object,
                          base::ScopedPtr<jscore::LynxArray>& array) {
        return base::ScopedPtr<jscore::LynxValue>();
    }

    base::ScopedPtr<jscore::LynxValue> Animation::Play(jscore::LynxObjectTemplate *object,
                         base::ScopedPtr<jscore::LynxArray>& array) {
        return base::ScopedPtr<jscore::LynxValue>();
    }

    void Animation::RemoveAllEventListener() {
        if (render_object_.IsValid()) {
            render_object_->RemoveEventListener(kEventFinish + id_);
            render_object_->RemoveEventListener(kEventCancel + id_);
        }
    }
}
