
#include "runtime/animation.h"
#include "runtime/base/lynx_array.h"
#include "runtime/base/lynx_value.h"
#include "runtime/js/class_template.h"
#include "runtime/js/class_template_storage.h"
#include "runtime/js/js_context.h"

namespace jscore {

    #define FOR_EACH_METHOD_BINDING(V)     \
        V(Animation, AddEventListener)     \
        V(Animation, RemoveEventListener)  \
        V(Animation, Play)                 \
        V(Animation, Pause)                \
        V(Animation, Cancel)               \
        V(Animation, Finish)

    // Defines methods and fields
    FOR_EACH_METHOD_BINDING(DEFINE_METHOD_CALLBACK)

    // Defines default ClassTemplate
    DEFINE_CLASS_TEMPLATE_START(Animation)
        FOR_EACH_METHOD_BINDING(REGISTER_METHOD_CALLBACK)
    DEFINE_CLASS_TEMPLATE_END

    const static std::string kEventFinish = "finish";
    const static std::string kEventCancel = "cancel";

    Animation::Animation(JSContext *context, const std::string &id)
            : LynxObject(context, DEFAULT_CLASS_TEMPLATE(context)),
              render_object_(NULL), id_(id) {

    }

    Animation::~Animation() {
        RemoveAllEventListener();
    }

    base::ScopedPtr<jscore::LynxValue>
    Animation::AddEventListener(base::ScopedPtr<jscore::LynxArray>& array) {
        if (array.Get() != NULL && array->Size() == 2
                && array->Get(0)->type_ == jscore::LynxValue::Type::VALUE_STRING
                && array->Get(1)->type_ == jscore::LynxValue::Type::VALUE_LYNX_FUNCTION
                && render_object_.IsValid()) {
            std::string event_name = array->Get(0)->ToString();
            if (event_name.compare(kEventFinish) == 0 || event_name.compare(kEventCancel) == 0) {
                render_object_->AddEventListener(event_name + id_,
                                                 array->Get(1)->data_.lynx_function,
                                                 false);
            }
        }
        return base::ScopedPtr<jscore::LynxValue>();
    }
    
    base::ScopedPtr<jscore::LynxValue>
    Animation::RemoveEventListener(base::ScopedPtr<jscore::LynxArray>& array) {
        if (array.Get() != NULL && array->Size() == 2
                && array->Get(0)->type_ == jscore::LynxValue::Type::VALUE_STRING
                && array->Get(1)->type_ == jscore::LynxValue::Type::VALUE_LYNX_FUNCTION
                && render_object_.IsValid()) {
            std::string event_name = array->Get(0)->ToString();
            if (event_name.compare(kEventFinish) == 0 || event_name.compare(kEventCancel) == 0) {
                render_object_->RemoveEventListener(event_name + id_,
                                                    array->Get(1)->data_.lynx_function);
            }
        }
        return base::ScopedPtr<jscore::LynxValue>();
    }

    base::ScopedPtr<jscore::LynxValue> Animation::Finish(base::ScopedPtr<jscore::LynxArray>& array) {

        return base::ScopedPtr<jscore::LynxValue>();
    }

    base::ScopedPtr<jscore::LynxValue> Animation::Cancel(base::ScopedPtr<jscore::LynxArray>& array) {
        if (render_object_.IsValid()) {
            render_object_->CancelAnimation(id_);
        }
        RemoveAllEventListener();
        return base::ScopedPtr<jscore::LynxValue>();
    }

    base::ScopedPtr<jscore::LynxValue> Animation::Pause(base::ScopedPtr<jscore::LynxArray>& array) {
        return base::ScopedPtr<jscore::LynxValue>();
    }

    base::ScopedPtr<jscore::LynxValue> Animation::Play(base::ScopedPtr<jscore::LynxArray>& array) {
        return base::ScopedPtr<jscore::LynxValue>();
    }

    void Animation::RemoveAllEventListener() {
        if (render_object_.IsValid()) {
            render_object_->RemoveEventListener(kEventFinish + id_);
            render_object_->RemoveEventListener(kEventCancel + id_);
        }
    }
}
