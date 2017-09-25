// Copyright 2017 The Lynx Authors. All rights reserved.

#include <runtime/base/lynx_value.h>
#include "runtime/document.h"
#include "runtime/base/lynx_array.h"
#include "render/render_factory.h"
#include "runtime/jsc/jsc_helper.h"
#include "runtime/js_context.h"
#include "runtime/jsc/jsc_context.h"
#include "runtime/runtime.h"
#include "runtime/element.h"

namespace jscore {
    Document::Document(JSContext* context) : context_(context) {
        set_class_name("Document");
        RegisterMethodCallback("createElement", &CreateElementCallback);
        RegisterMethodCallback("createDom", &CreateDomCallback);
        RegisterMethodCallback("createTextNode", &CreateTextNodeCallback);
        RegisterMethodCallback("addEventListener", &AddEventListenerCallback);
        RegisterMethodCallback("removeEventListener", &RemoveEventListenerCallback);
        RegisterMethodCallback("dispatchEvent", &DispatchEventCallback);
        RegisterMethodCallback("createEvent", &CreateEventCallback);
        RegisterMethodCallback("getElementById", &GetElementByIdCallback);
        RegisterMethodCallback("querySelector", &QuerySelectorCallback);

        RegisterAccessorCallback("domain", &GetDomainCallback, &SetDomainCallback);
        RegisterAccessorCallback("cookie", &GetCookieCallback, &SetCookieCallback);
        RegisterAccessorCallback("readyState", &GetReadyStateCallback, &SetReadyStateCallback);
        RegisterAccessorCallback("ontouchstart", 0, &SetOnTouchStartCallback);
        RegisterAccessorCallback("ontouchend", 0, &SetOnTouchEndCallback);
        RegisterAccessorCallback("ontouchmove", 0, &SetOnTouchMoveCallback);

    }

    Document::~Document() {

    }

    Element* Document::CreateElement(std::string &tag_name) {
        lynx::RenderObject* render_object
                = lynx::RenderFactory::CreateRenderObject(context_->runtime()->thread_manager(),
                                                          tag_name,
                                                          context_->runtime()->render_tree_host());

        return lynx_new Element(static_cast<JSCContext*>(context_), render_object);
    }

    Element* Document::CreateTextNode(std::string &text) {
        lynx::RenderObject* render_object
                = lynx::RenderFactory::CreateRenderObject(context_->runtime()->thread_manager(),
                                                          "text", context_->runtime()->render_tree_host());
        render_object->SetText(text);
        return lynx_new Element(static_cast<JSCContext*>(context_), render_object);
    }

    base::ScopedPtr<LynxValue>
    Document::QuerySelectorCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array) {
        Document* document = static_cast<Document*>(object);
        if(array.Get() != NULL && array->Size() > 0) {
            std::string text = array->Get(0)->data_.str;
            LynxValue *value = LynxValue::MakeObjectTemplate(document->QuerySelector(text));
            return base::ScopedPtr<LynxValue>(value);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    Element* Document::GetElementById(std::string &id) {
        lynx::RenderObject* render_object
                = context_->runtime()->render_tree_host()->GetElementById(id);
        return static_cast<jscore::Element*>(render_object->GetJSRef());
    }

    Element* Document::QuerySelector(std::string &element) {
        lynx::RenderObject* render_object = NULL;
        if(element[0] == '#') {
            render_object
                    = context_->runtime()->render_tree_host()->GetElementById(element.substr(1));
        }
        return render_object ? static_cast<jscore::Element*>(render_object->GetJSRef()) : NULL;
    }

    base::ScopedPtr<LynxValue>
    Document::CreateElementCallback(LynxObjectTemplate* object,
                                    base::ScopedPtr<LynxArray> array) {
        Document* document = static_cast<Document*>(object);
        if (array.Get() != NULL
            && array->Size() > 0
            && array->Get(0)->type_ == LynxValue::Type::VALUE_STRING) {
            std::string tag_name = array->Get(0)->data_.str;
            Element* element = document->CreateElement(tag_name);

            LynxValue *value = LynxValue::MakeObjectTemplate(element);
            return base::ScopedPtr<LynxValue>(value);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Document::CreateDomCallback(LynxObjectTemplate* object,
                                base::ScopedPtr<LynxArray> array) {
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Document::CreateTextNodeCallback(LynxObjectTemplate* object,
                                base::ScopedPtr<LynxArray> array) {
        Document* document = static_cast<Document*>(object);
        if (array.Get() != NULL
            && array->Size() > 0
            && array->Get(0)->type_ == LynxValue::Type::VALUE_STRING) {
            std::string text = array->Get(0)->data_.str;
            Element* element = document->CreateTextNode(text);

            LynxValue *value = LynxValue::MakeObjectTemplate(element);
            return base::ScopedPtr<LynxValue>(value);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Document::AddEventListenerCallback(LynxObjectTemplate* object,
                                       base::ScopedPtr<LynxArray> array) {
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Document::RemoveEventListenerCallback(LynxObjectTemplate* object,
                                          base::ScopedPtr<LynxArray> array) {

        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Document::DispatchEventCallback(LynxObjectTemplate* object,
                                    base::ScopedPtr<LynxArray> array) {
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Document::CreateEventCallback(LynxObjectTemplate* object,
                                  base::ScopedPtr<LynxArray> array) {
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Document::GetElementByIdCallback(LynxObjectTemplate* object,
                                     base::ScopedPtr<LynxArray> array) {
        Document* document = static_cast<Document*>(object);
        if (array.Get() != NULL
                && array->Size() > 0
                && array->Get(0)->type_ == LynxValue::Type::VALUE_STRING) {
            std::string id = array->Get(0)->data_.str;
            Element* element = document->GetElementById(id);
            LynxValue *value = LynxValue::MakeObjectTemplate(element);
            return base::ScopedPtr<LynxValue>(value);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Document::GetDomainCallback(LynxObjectTemplate* object) {
        LynxValue *value = LynxValue::MakeString("");
        return base::ScopedPtr<LynxValue>(value);
    }

    base::ScopedPtr<LynxValue> Document::GetCookieCallback(LynxObjectTemplate* object) {
        LynxValue *value = LynxValue::MakeString("");
        return base::ScopedPtr<LynxValue>(value);
    }

    base::ScopedPtr<LynxValue> Document::GetReadyStateCallback(LynxObjectTemplate* object) {
        return base::ScopedPtr<LynxValue>(NULL);
    }

    void Document::SetDomainCallback(LynxObjectTemplate* object,
                                  base::ScopedPtr<jscore::LynxValue> value) {

    }

    void Document::SetCookieCallback(LynxObjectTemplate* object,
                                  base::ScopedPtr<jscore::LynxValue> value) {

    }

    void Document::SetReadyStateCallback(LynxObjectTemplate* object,
                                      base::ScopedPtr<jscore::LynxValue> value) {

    }

    void Document::SetOnTouchStartCallback(LynxObjectTemplate* object,
                                        base::ScopedPtr<jscore::LynxValue> value) {

    }

    void Document::SetOnTouchEndCallback(LynxObjectTemplate* object,
                                      base::ScopedPtr<jscore::LynxValue> value) {

    }

    void Document::SetOnTouchMoveCallback(LynxObjectTemplate* object,
                                       base::ScopedPtr<jscore::LynxValue> value) {

    }
}