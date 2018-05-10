// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/document.h"
#include "render/render_factory.h"
#include "runtime/base/lynx_value.h"
#include "runtime/base/lynx_array.h"
#include "runtime/runtime.h"
#include "runtime/element.h"
#include "runtime/canvas.h"
#include "runtime/js/js_context.h"
#include "runtime/js/class_template.h"

namespace jscore {

    #define FOR_EACH_METHOD_BINDING(V)      \
        V(Document, CreateElement)          \
        V(Document, CreateDom)              \
        V(Document, CreateTextNode)         \
        V(Document, AddEventListener)       \
        V(Document, RemoveEventListener)    \
        V(Document, DispatchEvent)          \
        V(Document, CreateEvent)            \
        V(Document, GetElementById)         \
        V(Document, QuerySelector)

    #define FOR_EACH_FIELD_GET_BINDING(V)   \
        V(Document, Domain)                 \
        V(Document, Cookie)                 \
        V(Document, ReadyState)             \
        V(Document, Body)

    #define FOR_EACH_FIELD_SET_BINDING(V)   \
        V(Document, Domain)                 \
        V(Document, Cookie)                 \
        V(Document, ReadyState)             \
        V(Document, OnTouchStart)           \
        V(Document, OnTouchEnd)             \
        V(Document, OnTouchMove)

    // Defines methods and fields
    FOR_EACH_METHOD_BINDING(DEFINE_METHOD_CALLBACK)
    FOR_EACH_FIELD_GET_BINDING(DEFINE_GET_CALLBACK)
    FOR_EACH_FIELD_SET_BINDING(DEFINE_SET_CALLBACK)

    // Defines default ClassTemplate
    DEFINE_CLASS_TEMPLATE_START(Document)
        EXPOSE_CONSTRUCTOR(true)
        FOR_EACH_METHOD_BINDING(REGISTER_METHOD_CALLBACK)
        FOR_EACH_FIELD_GET_BINDING(REGISTER_GET_CALLBACK)
        FOR_EACH_FIELD_SET_BINDING(REGISTER_SET_CALLBACK)
    DEFINE_CLASS_TEMPLATE_END

    Document::Document(JSContext* context) : LynxObject(context, DEFAULT_CLASS_TEMPLATE(context)){
        body_ = lynx_new Element(context, context->runtime()->render_tree_host()->render_root());
    }

    Document::~Document() {

    }

    Element* Document::CreateElement(std::string &tag_name) {
        Element* element = NULL;
        lynx::RenderObject* render_object
                = lynx::RenderFactory::CreateRenderObject(context_->runtime()->thread_manager(),
                                                          tag_name,
                                                          context_->runtime()->render_tree_host());
        if(tag_name.compare("xcanvas") == 0) {
            element = lynx_new Canvas(context_, render_object);
        } else {
            element = lynx_new Element(context_, render_object);
        }
        return element;
    }

    Element* Document::CreateTextNode(std::string &text) {
        lynx::RenderObject* render_object
                = lynx::RenderFactory::CreateRenderObject(context_->runtime()->thread_manager(),
                                                          "text",
                                                          context_->runtime()->render_tree_host());
        render_object->SetText(text);
        return lynx_new Element(context_, render_object);
    }

    base::ScopedPtr<LynxValue> Document::QuerySelector(base::ScopedPtr<LynxArray>& array) {
        if(array.Get() != NULL && array->Size() > 0) {
            std::string text = array->Get(0)->data_.str;
            return LynxValue::MakeObject(QuerySelector(text));
    }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    Element* Document::GetElementById(std::string &id) {
        lynx::RenderObject* render_object
                = context_->runtime()->render_tree_host()->GetElementById(id);
        return render_object->GetJSRef();
    }

    Element* Document::QuerySelector(std::string &element) {
        lynx::RenderObject* render_object = NULL;
        if(element[0] == '#') {
            render_object
                    = context_->runtime()->render_tree_host()->GetElementById(element.substr(1));
        }
        return render_object ? render_object->GetJSRef() : NULL;
    }

    base::ScopedPtr<LynxValue> Document::CreateElement(base::ScopedPtr<LynxArray>& array) {
        if (array.Get() != NULL
            && array->Size() > 0
            && array->Get(0)->type_ == LynxValue::Type::VALUE_STRING) {
            std::string tag_name = array->Get(0)->data_.str;
            Element* element = CreateElement(tag_name);

            return LynxValue::MakeObject(element);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Document::CreateDom(base::ScopedPtr<LynxArray>& array) {
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Document::CreateTextNode(base::ScopedPtr<LynxArray>& array) {
        if (array.Get() != NULL
            && array->Size() > 0
            && array->Get(0)->type_ == LynxValue::Type::VALUE_STRING) {
            std::string text = array->Get(0)->data_.str;
            Element* element = CreateTextNode(text);

            return LynxValue::MakeObject(element);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Document::AddEventListener(base::ScopedPtr<LynxArray>& array) {
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Document::RemoveEventListener(base::ScopedPtr<LynxArray>& array) {
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Document::DispatchEvent(base::ScopedPtr<LynxArray>& array) {
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Document::CreateEvent(base::ScopedPtr<LynxArray>& array) {
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Document::GetElementById(base::ScopedPtr<LynxArray>& array) {
        if (array.Get() != NULL
                && array->Size() > 0
                && array->Get(0)->type_ == LynxValue::Type::VALUE_STRING) {
            std::string id = array->Get(0)->data_.str;
            Element* element = GetElementById(id);
            return LynxValue::MakeObject(element);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Document::GetDomain() {
        return LynxValue::MakeString("");
    }

    base::ScopedPtr<LynxValue> Document::GetCookie() {
        return LynxValue::MakeString("");
    }

    base::ScopedPtr<LynxValue> Document::GetReadyState() {
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Document::GetBody() {
        return LynxValue::MakeObject(body_);
    }

    void Document::SetDomain(base::ScopedPtr<jscore::LynxValue> value) {

    }

    void Document::SetCookie(base::ScopedPtr<jscore::LynxValue> value) {

    }

    void Document::SetReadyState(base::ScopedPtr<jscore::LynxValue> value) {

    }

    void Document::SetOnTouchStart(base::ScopedPtr<jscore::LynxValue> value) {

    }

    void Document::SetOnTouchEnd(base::ScopedPtr<jscore::LynxValue> value) {

    }

    void Document::SetOnTouchMove(base::ScopedPtr<jscore::LynxValue> value) {

    }
}