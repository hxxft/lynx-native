// Copyright 2017 The Lynx Authors. All rights reserved.

#include <sstream>
#include <runtime/base/lynx_value.h>
#include <algorithm>

#include "runtime/base/lynx_value.h"
#include "runtime/element.h"
#include "runtime/jsc/jsc_helper.h"
#include "runtime/jsc/objects/object_template.h"

#include "render/event_target.h"
#include "render/render_object.h"
#include "render/animation.h"

namespace jscore {

    //static int ElementObjectCount = 0;
    std::map<int,std::map<std::string, int>>  Element::s_rpc_methods;
    std::map<std::string,int>  Element::s_element_tags;
    Element::Element(JSContext* context, lynx::RenderObject* render_object)
            : is_protect_(false), render_object_(render_object), context_(context) {

        if(render_object_.Get() != NULL) {
            render_object_->SetJSRef(this);
        }

        std::string tag_name = render_object->tag_name().c_str();
        std::transform(tag_name.begin(),++(tag_name.begin()),tag_name.begin(), ::toupper);
        set_class_name(tag_name);

        RegisterMethodCallback("appendChild", &AppendChildCallback);
        RegisterMethodCallback("appendChildren", &AppendChildrenCallback);
        RegisterMethodCallback("insertChildAtIndex", &InsertChildAtIndexCallback);
        RegisterMethodCallback("removeChildByIndex", &RemoveChildByIndexCallback);
        RegisterMethodCallback("insertBefore", &InsertBeforeCallback);
        RegisterMethodCallback("removeChild", &RemoveChildCallback);
        RegisterMethodCallback("getChildByIndex", &GetChildByIndexCallback);
        RegisterMethodCallback("addEventListener", &AddEventListenerCallback);
        RegisterMethodCallback("removeEventListener", &RemoveEventListenerCallback);
        RegisterMethodCallback("setAttribution", &SetAttributionCallback);
        RegisterMethodCallback("setAttribute", &SetAttributeCallback);
        RegisterMethodCallback("hasAttribute", &HasAttributeCallback);
        RegisterMethodCallback("removeAttribute", &RemoveAttributeCallback);
        RegisterMethodCallback("setStyle", &SetStyleCallback);
        RegisterMethodCallback("setText", &SetTextCallback);
        RegisterMethodCallback("getText", &GetTextCallback);
        RegisterMethodCallback("hasChildNodes", &HasChildNodesCallback);
        RegisterMethodCallback("animate", &Animate);

        SetAllExtraMethod();

        RegisterAccessorCallback("tagName", &GetTagNameCallback, 0);
        RegisterAccessorCallback("nodeType", &GetNodeTypeCallback, 0);
        RegisterAccessorCallback("parentNode", &GetParentNodeCallback, 0);
        RegisterAccessorCallback("offsetTop", &GetOffsetTopCallback, 0);
        RegisterAccessorCallback("offsetLeft", &GetOffsetLeftCallback, 0);
        RegisterAccessorCallback("offsetWidth", &GetOffsetWidthCallback, 0);
        RegisterAccessorCallback("offsetHeight", &GetOffsetHeightCallback, 0);
        RegisterAccessorCallback("scrollWidth", &GetScrollWidthCallback, 0);
        RegisterAccessorCallback("scrollHeight", &GetScrollHeightCallback, 0);
        RegisterAccessorCallback("scrollTop", &GetScrollTopCallback, &SetScrollTopCallback);
        RegisterAccessorCallback("scrollLeft", &GetScrollLeftCallback, &SetScrollLeftCallback);
        RegisterAccessorCallback("nextSibling", &GetNextSiblingCallback, 0);
        RegisterAccessorCallback("clientWidth", &GetClientWidthCallback, 0);
        RegisterAccessorCallback("clientHeight", &GetClientHeightCallback, 0);
        RegisterAccessorCallback("textContent", &GetTextContentCallback, &SetTextContentCallback);
        RegisterAccessorCallback("childNodes", &GetChildNodesCallback, 0);
        RegisterAccessorCallback("firstChild", &GetFirstChildCallback, 0);
    }

    Element::~Element() {

    }

    base::ScopedPtr<LynxValue>
    Element::AppendChildCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array) {
        Element *element = static_cast<Element *>(object);
        if (array.Get() != NULL && array->Size() > 0) {
            lynx::RenderObject* render_object = element->render_object();
            Element* child_element =
                    static_cast<Element*>(array->Get(0)->data_.lynx_object_template);
            lynx::RenderObject* render_child = child_element->render_object();
            render_object->AppendChild(render_child);

            // Protects child
            Element::ProtectChild(element->context(), child_element);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Element::AppendChildrenCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array) {
        Element *element = static_cast<Element *>(object);
        if (array.Get() != NULL && array->Size() > 0) {
            lynx::RenderObject* render_object = element->render_object();
            for (int i = 0; i < array->Size(); ++i) {
                Element* child_element =
                        static_cast<Element*>(array->Get(i)->data_.lynx_object_template);
                if(child_element != NULL) break;
                lynx::RenderObject* render_child = child_element->render_object();
                render_object->AppendChild(render_child);

                // Protects child
                Element::ProtectChild(element->context(), child_element);
            }
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Element::InsertChildAtIndexCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array) {
        Element *element = static_cast<Element *>(object);
        if (array.Get() != NULL && array->Size() > 1) {
            lynx::RenderObject* render_object = element->render_object();
            Element* child_element =
                    static_cast<Element*>(array->Get(0)->data_.lynx_object_template);
            lynx::RenderObject* render_child = child_element->render_object();
            int index = array->Get(1)->data_.i;
            render_object->InsertChild(render_child, index);

            // Protects child
            Element::ProtectChild(element->context(), child_element);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Element::RemoveChildByIndexCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array) {
        Element *element = static_cast<Element *>(object);
        if (array.Get() != NULL && array->Size() > 0) {
            lynx::RenderObject* render_parent = element->render_object();
            int index = array->Get(0)->data_.i;
            lynx::RenderObject* render_child = const_cast<lynx::RenderObject*>(
                    render_parent->Get(index));
            Element* child_element = render_child->GetJSRef();
            // Unprotects child
            Element::UnprotectChild(element->context(), child_element);
            render_parent->RemoveChild(render_child);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Element::InsertBeforeCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array) {
        Element *element = static_cast<Element *>(object);
        if (array.Get() != NULL && array->Size() > 0) {
            lynx::RenderObject* render_object = element->render_object();
            Element* child_element =
                    static_cast<Element*>(array->Get(0)->data_.lynx_object_template);
            lynx::RenderObject* child = child_element->render_object();
            if (array->Size() > 1 && array->Get(1) != NULL) {
                Element* reference_element =
                        static_cast<Element*>(array->Get(1)->data_.lynx_object_template);
                lynx::RenderObject* reference = reference_element->render_object();
                render_object->InsertBefore(child, reference);
            } else {
                render_object->AppendChild(child);
            }

            Element::ProtectChild(element->context(), child_element);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Element::RemoveChildCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array) {
        Element *element = static_cast<Element *>(object);
        if (array.Get() != NULL && array->Size() > 0) {
            lynx::RenderObject* render_object = element->render_object();
            Element* child_element =
                    static_cast<Element*>(array->Get(0)->data_.lynx_object_template);
            lynx::RenderObject* child = child_element->render_object();
            render_object->RemoveChild(child);
            Element::UnprotectChild(element->context(), child_element);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Element::GetChildByIndexCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array) {
        Element *element = static_cast<Element *>(object);
        if (array.Get() != NULL && array->Size() > 0) {
            lynx::RenderObject* render_object = element->render_object_.Get();
            int index = array->Get(0)->data_.i;
            lynx::RenderObject* child = const_cast<lynx::RenderObject*>(render_object->Get(index));
            if(child) {
                Element* child_element = child->GetJSRef();
                return base::ScopedPtr<LynxValue>(LynxValue::MakeObjectTemplate(child_element));
            }
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Element::AddEventListenerCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array) {
        Element *element = static_cast<Element *>(object);
        if (array.Get() != NULL && array->Size() > 1) {
            lynx::RenderObject* render_object = element->render_object();
            std::string event = array->Get(0)->data_.str;
            bool capture = false;
            if (array->Size() > 2 && array->Get(2)->type_ == LynxValue::Type::VALUE_BOOL) {
                capture = array->Get(2)->data_.b;
            }
            LynxFunction* js_function = array->Get(1)->data_.lynx_function;
            render_object->AddEventListener(event, js_function, capture);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Element::RemoveEventListenerCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array) {
        Element *element = static_cast<Element *>(object);
        if (array.Get() != NULL && array->Size() > 1) {
            lynx::RenderObject* render_object = element->render_object();
            std::string event = array->Get(0)->data_.str;
            LynxFunction* js_function = array->Get(1)->data_.lynx_function;
            render_object->RemoveEventListener(event, js_function);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Element::SetAttributionCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array) {
        Element *element = static_cast<Element *>(object);
        if (array.Get() != NULL
            && array->Size() > 0
            && array->Get(0)->type_ == LynxValue::Type::VALUE_LYNX_OBJECT) {
            lynx::RenderObject *render_object = element->render_object();
            LynxObject* lynx_object = array->Get(0)->data_.lynx_object;
            for (int i = 0; i < lynx_object->Size(); ++i) {
                std::string key = lynx_object->GetName(i);
                LynxValue *value = lynx_object->GetProperty(key);
                render_object->SetAttribute(key, JSCHelper::ConvertToString(value));
            }
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Element::SetAttributeCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array) {
        Element *element = static_cast<Element *>(object);
        if(array.Get() != NULL && array->Size() == 2
           && array->Get(0)->type_ == LynxValue::VALUE_STRING) {
            lynx::RenderObject* render_object = element->render_object();
            render_object->SetAttribute(array->Get(0)->data_.str,
                                        JSCHelper::ConvertToString(array->Get(1)));
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Element::HasAttributeCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array) {
        Element *element = static_cast<Element *>(object);
        if(array.Get() != NULL && array->Size() == 1) {
            lynx::RenderObject* render_object = element->render_object();
            bool has_attribute = render_object->HasAttribute(array->Get(0)->data_.str);
            return base::ScopedPtr<LynxValue>(LynxValue::MakeBool(has_attribute));
        }
        return base::ScopedPtr<LynxValue>(LynxValue::MakeBool(false));
    }

    base::ScopedPtr<LynxValue>
    Element::RemoveAttributeCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array) {
        Element *element = static_cast<Element *>(object);
        if(array.Get() != NULL && array->Size() == 1) {
            lynx::RenderObject* render_object = element->render_object();
            render_object->RemoveAttribute(array->Get(0)->data_.str);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Element::SetStyleCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array) {

        if (array.Get() == NULL) return base::ScopedPtr<LynxValue>(NULL);

        Element *element = static_cast<Element *>(object);
        lynx::RenderObject* render_object = element->render_object();

        if(array->Size() == 2) {
            // Key and value for style
            render_object->SetStyle(JSCHelper::ConvertToString(array->Get(0)),
                                    JSCHelper::ConvertToString(array->Get(1)));
        } else if (array->Size() == 1
                   && array->Get(0)->type_ == jscore::LynxValue::Type::VALUE_LYNX_OBJECT){
            // Object contains pairs of key and value for style
            LynxObject* lynx_object = array->Get(0)->data_.lynx_object;
            for(int i = 0; i < lynx_object->Size(); ++i) {
                std::string key = lynx_object->GetName(i);
                LynxValue *value = lynx_object->GetProperty(key);
                render_object->SetStyle(key, JSCHelper::ConvertToString(value));
            }
            render_object->SetStyle("", "");
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Element::SetTextCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array) {
        Element *element = static_cast<Element *>(object);
        if(array.Get() != NULL && array->Size() > 0) {
            lynx::RenderObject* render_object = element->render_object();
            std::string text = JSCHelper::ConvertToString(array->Get(0));
            render_object->SetText(text);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Element::GetTextCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array) {
        Element *element = static_cast<Element *>(object);
        lynx::RenderObject* render_object = element->render_object_.Get();
        return base::ScopedPtr<LynxValue>(LynxValue::MakeString(render_object->GetText().c_str()));
    }

    JSValueRef
    Element::ExtraCallback (JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject,
                            size_t argumentCount, const JSValueRef arguments[],
                            JSValueRef* exception) {

        JSStringRef name_key = JSStringCreateWithUTF8CString("name");
        auto  func_property = JSObjectGetProperty(ctx,function,name_key,NULL);
        auto function_name = JSCHelper::ConvertToString(ctx, func_property);
        JSStringRelease(name_key);
        ObjectTemplate *obj = ObjectWrap::Unwrap<ObjectTemplate>(thisObject);
        Element *element = static_cast<Element *>(obj->target());
        lynx::RenderObject *render_object = element->render_object();
        auto method_spec_iter = element->element_methods.find(function_name);
        if (method_spec_iter != element->element_methods.end()) {
            int method_id = method_spec_iter->second;
            base::ScopedPtr<LynxValue> property_scoped_ptr;
            if (argumentCount > 0) {
                property_scoped_ptr =
                        JSCHelper::ConvertToLynxArray(ctx, const_cast<JSValueRef *>(arguments),
                                                      argumentCount);
            }
            render_object->SetData(method_id, property_scoped_ptr);
        }
        return JSCHelper::ConvertToJSValue(ctx, NULL);
    }

    base::ScopedPtr<LynxValue>
    Element::HasChildNodesCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array) {
        Element* element = static_cast<Element*>(object);
        lynx::RenderObject* render_object = element->render_object();

        return base::ScopedPtr<LynxValue>(LynxValue::MakeBool(render_object->GetChildCount() != 0));
    }

    base::ScopedPtr<LynxValue> Element::GetTagNameCallback(LynxObjectTemplate* object) {
        Element *element = static_cast<Element *>(object);
        return LynxValue::MakeString(element->render_object()->tag_name());
    }

    base::ScopedPtr<LynxValue> Element::GetNodeTypeCallback(LynxObjectTemplate* object) {
        return LynxValue::MakeInt(1);
    }

    base::ScopedPtr<LynxValue> Element::GetParentNodeCallback(LynxObjectTemplate* object) {
        Element *element = static_cast<Element *>(object);
        lynx::RenderObject* render_object = element->render_object();
        lynx::RenderObject* parent = const_cast<lynx::RenderObject*>(render_object->Parent());
        if (parent) {
            Element* parent_element = parent->GetJSRef();
            return LynxValue::MakeObjectTemplate(parent_element);
        }
        return LynxValue::MakeValueScoped(NULL);
    }

    base::ScopedPtr<LynxValue> Element::GetOffsetTopCallback(LynxObjectTemplate* object) {
        Element *element = static_cast<Element *>(object);
        return LynxValue::MakeInt(element->render_object()->offset_top());
    }

    base::ScopedPtr<LynxValue> Element::GetOffsetLeftCallback(LynxObjectTemplate* object) {
        Element *element = static_cast<Element *>(object);
        return LynxValue::MakeInt(element->render_object()->offset_left());
    }

    base::ScopedPtr<LynxValue> Element::GetOffsetWidthCallback(LynxObjectTemplate* object) {
        Element *element = static_cast<Element *>(object);
        return LynxValue::MakeInt(element->render_object()->offset_width());
    }

    base::ScopedPtr<LynxValue> Element::GetOffsetHeightCallback(LynxObjectTemplate* object) {
        Element *element = static_cast<Element *>(object);
        return LynxValue::MakeInt(element->render_object()->offset_height());
    }

    base::ScopedPtr<LynxValue> Element::GetScrollWidthCallback(LynxObjectTemplate* object) {
        Element *element = static_cast<Element *>(object);
        return LynxValue::MakeInt(element->render_object()->scroll_width());
    }

    base::ScopedPtr<LynxValue> Element::GetScrollHeightCallback(LynxObjectTemplate* object) {
        Element *element = static_cast<Element *>(object);
        return LynxValue::MakeInt(element->render_object()->scroll_height());
    }

    base::ScopedPtr<LynxValue> Element::GetScrollTopCallback(LynxObjectTemplate* object) {
        Element *element = static_cast<Element *>(object);
        return LynxValue::MakeInt(element->render_object()->scroll_top());
    }

    base::ScopedPtr<LynxValue> Element::GetScrollLeftCallback(LynxObjectTemplate* object) {
        Element *element = static_cast<Element *>(object);
        return LynxValue::MakeInt(element->render_object()->scroll_left());
    }

    base::ScopedPtr<LynxValue> Element::GetTextContentCallback(LynxObjectTemplate* object) {
        Element *element = static_cast<Element *>(object);
        return LynxValue::MakeString(element->render_object()->GetText());
    }

    void Element::SetTextContentCallback(LynxObjectTemplate* object,
                                                               base::ScopedPtr<jscore::LynxValue> value) {
        Element* element = static_cast<Element*>(object);
        lynx::RenderObject* render_object = element->render_object();
        render_object->SetText(value->data_.str);
    }

    base::ScopedPtr<LynxValue> Element::GetNextSiblingCallback(LynxObjectTemplate* object) {
        Element* element = static_cast<Element*>(object);
        lynx::RenderObject* render_object = element->render_object();
        lynx::RenderObject* next = render_object->NextSibling();
        if(next) {
            Element* next_element = next->GetJSRef();
            return LynxValue::MakeObjectTemplate(next_element);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Element::GetClientHeightCallback(LynxObjectTemplate *object) {
        int height = config::GlobalConfigData::GetInstance()->screen_height();
        return base::ScopedPtr<LynxValue>(LynxValue::MakeInt(height));
    }

    base::ScopedPtr<LynxValue> Element::GetClientWidthCallback(LynxObjectTemplate *object) {
        int width = config::GlobalConfigData::GetInstance()->screen_width();
        return base::ScopedPtr<LynxValue>(LynxValue::MakeInt(width));
    }

    base::ScopedPtr<LynxValue> Element::GetFirstChildCallback(LynxObjectTemplate* object) {
        Element* element = static_cast<Element*>(object);
        lynx::RenderObject* render_object = element->render_object();
        lynx::RenderObject* render_child = static_cast<lynx::RenderObject*>(render_object->FirstChild());
        if(render_child) {
            render_child = render_child->IsPrivate() ?
                           static_cast<lynx::RenderObject*>(render_child->FirstChild())
                            : render_child;
            Element* next_element = render_child->GetJSRef();
            return LynxValue::MakeObjectTemplate(next_element);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Element::GetChildNodesCallback(LynxObjectTemplate* object) {
        Element* element = static_cast<Element*>(object);
        lynx::RenderObject* render_object = element->render_object();
        lynx::RenderObject* render_child = static_cast<lynx::RenderObject*>(render_object->FirstChild());

        LynxArray* array = lynx_new LynxArray();
        if(render_object->GetChildCount() == 0) {
            return LynxValue::MakeValueScoped(array);
        }

        while (render_child) {
            Element* child_element = NULL;

            if (render_child->IsPrivate()) {
                lynx::RenderObject* render_temp =
                        static_cast<lynx::RenderObject*>(render_child->FirstChild());
                child_element = render_temp->GetJSRef();
            } else {
                child_element = render_child->GetJSRef();
            }

            array->Push(LynxValue::MakeObjectTemplate(child_element).Release());
            render_child = static_cast<lynx::RenderObject*>(render_child->Next());
        }
        return LynxValue::MakeValueScoped(array);
    }

    void Element::SetScrollTopCallback(LynxObjectTemplate* object,
                                     base::ScopedPtr<jscore::LynxValue> value) {
        Element* element = static_cast<Element*>(object);
        lynx::RenderObject* render_object = element->render_object();
        int scroll_top = value->data_.i;
        render_object->SetScrollTop(scroll_top);
    }

    void Element::SetScrollLeftCallback(LynxObjectTemplate* object,
                                      base::ScopedPtr<jscore::LynxValue> value) {
        Element* element = static_cast<Element*>(object);
        lynx::RenderObject* render_object = element->render_object();
        int scroll_left = value->data_.i;
        render_object->SetScrollLeft(scroll_left);
    }

    base::ScopedPtr<LynxValue>
    Element::Animate(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array) {
        Element* element = static_cast<Element*>(object);
        lynx::RenderObject* render_object = element->render_object();
        if (array.Get() != NULL && array->Size() == 2
                && array->Get(0)->type_ == LynxValue::Type::VALUE_LYNX_ARRAY
                && array->Get(1)->type_ == LynxValue::Type::VALUE_LYNX_OBJECT) {
            base::ScopedPtr<LynxArray> keyframes =
                    LynxValue::MakeArrayScoped(array->Release(0).Release()->data_.lynx_array);
            base::ScopedPtr<LynxObject> options =
                    LynxValue::MakeObjectScoped(array->Release(1).Release()->data_.lynx_object);
            return LynxValue::MakeObjectTemplate(render_object->Animate(keyframes, options).Release());
        }
        return base::ScopedPtr<LynxValue>();
    }

    void Element::ProtectChild(JSContext* context, Element* child) {
        if (child->object_wrap() == NULL) {
            // Create JSObject immediately
            JSCHelper::ConvertToJSObject(static_cast<JSCContext*>(context)->GetContext(),
                                                 child);
        }
        child->object_wrap()->Protect();
    }

    void Element::UnprotectChild(JSContext* context, Element* child) {
        if (child->object_wrap() == NULL) {
            // Create JSObject immediately
            JSCHelper::ConvertToJSObject(static_cast<JSCContext*>(context)->GetContext(),
                                                 child);
        }
        child->object_wrap()->Unprotect();
        lynx::RenderObject* renderer = child->render_object();
        lynx::RenderObject* render_child = static_cast<lynx::RenderObject*>(renderer->FirstChild());
        while(render_child) {
            UnprotectChild(context, render_child->GetJSRef());
            render_child = static_cast<lynx::RenderObject*>(renderer->Next());
        }
    }

    void Element::SetAllExtraMethod() {
        int type = render_object_->render_object_type();
        auto methods =  s_rpc_methods.find(type);
        if(methods != s_rpc_methods.end()){
           element_methods = methods->second;
           std::map<std::string,int>::iterator method_itr;
           for (method_itr = element_methods.begin();
                method_itr != element_methods.end(); ++method_itr) {
                RegisterRawMethodCallback(method_itr->first, &ExtraCallback);
            }
        }
    }
}
