// Copyright 2017 The Lynx Authors. All rights reserved.

#include <sstream>
#include <algorithm>

#include "runtime/element.h"
#include "runtime/animation.h"
#include "runtime/base/lynx_value.h"
#include "runtime/base/lynx_array.h"
#include "runtime/js/class_template.h"
#include "runtime/js/js_context.h"

#include "render/event_target.h"
#include "render/render_object.h"

namespace jscore {

    DEFINE_GROUP_METHOD_CALLBACK(Element, Extra)

    #define FOR_EACH_METHOD_BINDING(V)   \
        V(Element, AppendChild)          \
        V(Element, AppendChildren)       \
        V(Element, InsertChildAtIndex)   \
        V(Element, RemoveChildByIndex)   \
        V(Element, InsertBefore)         \
        V(Element, RemoveChild)          \
        V(Element, GetChildByIndex)      \
        V(Element, AddEventListener)     \
        V(Element, RemoveEventListener)  \
        V(Element, SetAttribution)       \
        V(Element, SetAttribute)         \
        V(Element, HasAttribute)         \
        V(Element, RemoveAttribute)      \
        V(Element, SetStyle)             \
        V(Element, SetText)              \
        V(Element, GetText)              \
        V(Element, HasChildNodes)        \
        V(Element, Animate)

    #define FOR_EACH_FIELD_GET_BINDING(V)   \
        V(Element, TagName)                 \
        V(Element, NodeType)                \
        V(Element, ParentNode)              \
        V(Element, OffsetTop)               \
        V(Element, OffsetLeft)              \
        V(Element, OffsetWidth)             \
        V(Element, OffsetHeight)            \
        V(Element, ScrollWidth)             \
        V(Element, ScrollHeight)            \
        V(Element, ScrollTop)               \
        V(Element, ScrollLeft)              \
        V(Element, TextContent)             \
        V(Element, NextSibling)             \
        V(Element, ClientWidth)             \
        V(Element, ClientHeight)            \
        V(Element, ChildNodes)              \
        V(Element, FirstChild)

    #define FOR_EACH_FIELD_SET_BINDING(V)   \
        V(Element, TextContent)             \
        V(Element, ScrollTop)               \
        V(Element, ScrollLeft)

    // Defines methods and fields
    FOR_EACH_METHOD_BINDING(DEFINE_METHOD_CALLBACK)
    FOR_EACH_FIELD_GET_BINDING(DEFINE_GET_CALLBACK)
    FOR_EACH_FIELD_SET_BINDING(DEFINE_SET_CALLBACK)

    // Defines default ClassTemplate
    DEFINE_CLASS_TEMPLATE_START(Element)
        EXPOSE_CONSTRUCTOR(true)
        FOR_EACH_METHOD_BINDING(REGISTER_METHOD_CALLBACK)
        FOR_EACH_FIELD_GET_BINDING(REGISTER_GET_CALLBACK)
        FOR_EACH_FIELD_SET_BINDING(REGISTER_SET_CALLBACK)
    DEFINE_CLASS_TEMPLATE_END

    //static int ElementObjectCount = 0;
    std::map<int,std::map<std::string, int>>  Element::s_rpc_methods;
    std::map<std::string, int>  Element::s_element_tags;

    Element::Element(JSContext* context) : LynxObject(context, DEFAULT_CLASS_TEMPLATE(context)){

    }

    Element::Element(JSContext* context, lynx::RenderObject* render_object)
            : LynxObject(context, DEFAULT_CLASS_TEMPLATE(context)),
              render_object_(render_object) {

        if(render_object_.Get() != NULL) {
            render_object_->SetJSRef(this);
        }
        // For different tag element, there is a different class template
        std::string tag_name = render_object->tag_name().c_str();
        ClassTemplate* class_template = ClassTemplate::Generate(context, tag_name);
        if (!class_template->is_creation_finished()) {
            class_template->set_parent_class_template(DEFAULT_CLASS_TEMPLATE(context));
            SetAllExtraMethod(class_template);
            class_template->FinishCreation();
        }
        set_class_template(class_template);
    }

    Element::~Element() {

    }

    base::ScopedPtr<LynxValue> Element::AppendChild(base::ScopedPtr<LynxArray>& array) {
        if (array.Get() != NULL && array->Size() > 0) {
            Element* child_element =
                    static_cast<Element*>(array->Get(0)->data_.lynx_object);
            lynx::RenderObject* render_child = child_element->render_object();
            render_object_->AppendChild(render_child);

            // Protects child
            child_element->ProtectJSObject();
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Element::AppendChildren(base::ScopedPtr<LynxArray>& array) {
        if (array.Get() != NULL && array->Size() > 0) {
            for (int i = 0; i < array->Size(); ++i) {
                Element* child_element =
                        static_cast<Element*>(array->Get(i)->data_.lynx_object);
                if(child_element != NULL) break;
                lynx::RenderObject* render_child = child_element->render_object();
                render_object_->AppendChild(render_child);

                // Protects child
                child_element->ProtectJSObject();
            }
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Element::InsertChildAtIndex(base::ScopedPtr<LynxArray>& array) {
        if (array.Get() != NULL && array->Size() > 1) {
            Element* child_element =
                    static_cast<Element*>(array->Get(0)->data_.lynx_object);
            lynx::RenderObject* render_child = child_element->render_object();
            int index = array->Get(1)->data_.i;
            render_object_->InsertChild(render_child, index);

            // Protects child
            child_element->ProtectJSObject();
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Element::RemoveChildByIndex(base::ScopedPtr<LynxArray>& array) {
        if (array.Get() != NULL && array->Size() > 0) {
            int index = array->Get(0)->data_.i;
            lynx::RenderObject* render_child = const_cast<lynx::RenderObject*>(
                    render_object_->Get(index));
            Element* child_element = render_child->GetJSRef();
            // Unprotects child
            child_element->UnprotectJSObject();
            render_object_->RemoveChild(render_child);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Element::InsertBefore(base::ScopedPtr<LynxArray>& array) {
        if (array.Get() != NULL && array->Size() > 0) {
            Element* child_element =
                    static_cast<Element*>(array->Get(0)->data_.lynx_object);
            lynx::RenderObject* child = child_element->render_object();
            if (array->Size() > 1 && array->Get(1) != NULL) {
                Element* reference_element =
                        static_cast<Element*>(array->Get(1)->data_.lynx_object);
                lynx::RenderObject* reference = reference_element->render_object();
                render_object_->InsertBefore(child, reference);
            } else {
                render_object_->AppendChild(child);
            }

            child_element->ProtectJSObject();
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Element::RemoveChild(base::ScopedPtr<LynxArray>& array) {
        if (array.Get() != NULL && array->Size() > 0) {
            Element* child_element =
                    static_cast<Element*>(array->Get(0)->data_.lynx_object);
            lynx::RenderObject* child = child_element->render_object();
            render_object_->RemoveChild(child);
            child_element->UnprotectJSObject();
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Element::GetChildByIndex(base::ScopedPtr<LynxArray>& array) {
        if (array.Get() != NULL && array->Size() > 0) {
            int index = array->Get(0)->data_.i;
            lynx::RenderObject* child = const_cast<lynx::RenderObject*>(render_object_->Get(index));
            if(child) {
                Element* child_element = child->GetJSRef();
                return base::ScopedPtr<LynxValue>(LynxValue::MakeObject(child_element));
            }
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Element::AddEventListener(base::ScopedPtr<LynxArray>& array) {
        if (array.Get() != NULL && array->Size() > 1) {
            std::string event = array->Get(0)->data_.str;
            bool capture = false;
            if (array->Size() > 2 && array->Get(2)->type_ == LynxValue::Type::VALUE_BOOL) {
                capture = array->Get(2)->data_.b;
            }
            LynxFunction* js_function = array->Get(1)->data_.lynx_function;
            render_object_->AddEventListener(event, js_function, capture);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Element::RemoveEventListener(base::ScopedPtr<LynxArray>& array) {
        if (array.Get() != NULL && array->Size() > 1) {
            std::string event = array->Get(0)->data_.str;
            LynxFunction* js_function = array->Get(1)->data_.lynx_function;
            render_object_->RemoveEventListener(event, js_function);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Element::SetAttribution(base::ScopedPtr<LynxArray>& array) {
        if (array.Get() != NULL
            && array->Size() > 0
            && array->Get(0)->type_ == LynxValue::Type::VALUE_LYNX_MAP) {
            LynxMap* lynx_object = array->Get(0)->data_.lynx_map;
            for (int i = 0; i < lynx_object->Size(); ++i) {
                std::string key = lynx_object->GetName(i);
                LynxValue *value = lynx_object->GetProperty(key);
                render_object_->SetAttribute(key, value->ToString());
            }
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Element::SetAttribute(base::ScopedPtr<LynxArray>& array) {
        if(array.Get() != NULL && array->Size() == 2
           && array->Get(0)->type_ == LynxValue::VALUE_STRING) {
            render_object_->SetAttribute(array->Get(0)->data_.str, array->Get(1)->ToString());
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Element::HasAttribute(base::ScopedPtr<LynxArray>& array) {
        if(array.Get() != NULL && array->Size() == 1) {
            bool has_attribute = render_object_->HasAttribute(array->Get(0)->data_.str);
            return base::ScopedPtr<LynxValue>(LynxValue::MakeBool(has_attribute));
        }
        return base::ScopedPtr<LynxValue>(LynxValue::MakeBool(false));
    }

    base::ScopedPtr<LynxValue> Element::RemoveAttribute(base::ScopedPtr<LynxArray>& array) {
        if(array.Get() != NULL && array->Size() == 1) {
            render_object_->RemoveAttribute(array->Get(0)->data_.str);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Element::SetStyle(base::ScopedPtr<LynxArray>& array) {

        if (array.Get() == NULL) return base::ScopedPtr<LynxValue>(NULL);

        if(array->Size() == 2) {
            // Key and value for style
            render_object_->SetStyle(array->Get(0)->ToString(),
                                    array->Get(1)->ToString());
        } else if (array->Size() == 1
                   && array->Get(0)->type_ == jscore::LynxValue::Type::VALUE_LYNX_MAP){
            // Object contains pairs of key and value for style
            LynxMap* lynx_object = array->Get(0)->data_.lynx_map;
            for(int i = 0; i < lynx_object->Size(); ++i) {
                std::string key = lynx_object->GetName(i);
                LynxValue *value = lynx_object->GetProperty(key);
                render_object_->SetStyle(key, value->ToString());
            }
            render_object_->SetStyle("", "");
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Element::SetText(base::ScopedPtr<LynxArray>& array) {
        if(array.Get() != NULL && array->Size() > 0) {
            std::string text = array->Get(0)->ToString();
            render_object_->SetText(text);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Element::GetText(base::ScopedPtr<LynxArray>& array) {
        return base::ScopedPtr<LynxValue>(LynxValue::MakeString(render_object_->GetText().c_str()));
    }


    base::ScopedPtr<LynxValue>
    Element::Extra(const std::string& function, base::ScopedPtr<LynxArray>& array) {
        auto method_spec_iter = element_methods.find(function);
        if (method_spec_iter != element_methods.end()) {
            int method_id = method_spec_iter->second;
            base::ScopedPtr<LynxValue> property_scoped_ptr;
            render_object_->SetData(method_id, std::move(array));
        }
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Element::HasChildNodes(base::ScopedPtr<LynxArray>& array) {
        return base::ScopedPtr<LynxValue>(LynxValue::MakeBool(render_object_->GetChildCount() != 0));
    }

    base::ScopedPtr<LynxValue> Element::GetTagName() {
        return LynxValue::MakeString(render_object_->tag_name());
    }

    base::ScopedPtr<LynxValue> Element::GetNodeType() {
        return LynxValue::MakeInt(render_object_->GetNodeType());
    }

    base::ScopedPtr<LynxValue> Element::GetParentNode() {
        lynx::RenderObject* parent = const_cast<lynx::RenderObject*>(render_object_->Parent());
        if (parent) {
            Element* parent_element = parent->GetJSRef();
            return LynxValue::MakeObject(parent_element);
        }
        return LynxValue::MakeValueScoped(NULL);
    }

    base::ScopedPtr<LynxValue> Element::GetOffsetTop() {
        return LynxValue::MakeInt(render_object_->offset_top());
    }

    base::ScopedPtr<LynxValue> Element::GetOffsetLeft() {
        return LynxValue::MakeInt(render_object_->offset_left());
    }

    base::ScopedPtr<LynxValue> Element::GetOffsetWidth() {
        return LynxValue::MakeInt(render_object_->offset_width());
    }

    base::ScopedPtr<LynxValue> Element::GetOffsetHeight() {
        return LynxValue::MakeInt(render_object_->offset_height());
    }

    base::ScopedPtr<LynxValue> Element::GetScrollWidth() {
        return LynxValue::MakeInt(render_object_->scroll_width());
    }

    base::ScopedPtr<LynxValue> Element::GetScrollHeight() {
        return LynxValue::MakeInt(render_object_->scroll_height());
    }

    base::ScopedPtr<LynxValue> Element::GetScrollTop() {
        return LynxValue::MakeInt(render_object_->scroll_top());
    }

    base::ScopedPtr<LynxValue> Element::GetScrollLeft() {
        return LynxValue::MakeInt(render_object_->scroll_left());
    }

    base::ScopedPtr<LynxValue> Element::GetTextContent() {
        return LynxValue::MakeString(render_object_->GetText());
    }

    void Element::SetTextContent(base::ScopedPtr<jscore::LynxValue> value) {
        render_object_->SetText(value->data_.str);
    }

    base::ScopedPtr<LynxValue> Element::GetNextSibling() {
        lynx::RenderObject* next = render_object_->NextSibling();
        if(next) {
            Element* next_element = next->GetJSRef();
            return LynxValue::MakeObject(next_element);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Element::GetClientHeight() {
        int height = config::GlobalConfigData::GetInstance()->screen_height();
        return base::ScopedPtr<LynxValue>(LynxValue::MakeInt(height));
    }

    base::ScopedPtr<LynxValue> Element::GetClientWidth() {
        int width = config::GlobalConfigData::GetInstance()->screen_width();
        return base::ScopedPtr<LynxValue>(LynxValue::MakeInt(width));
    }

    base::ScopedPtr<LynxValue> Element::GetFirstChild() {
        lynx::RenderObject* render_child =
                static_cast<lynx::RenderObject*>(render_object_->FirstChild());
        if(render_child) {
            render_child = render_child->IsPrivate() ?
                           static_cast<lynx::RenderObject*>(render_child->FirstChild())
                            : render_child;
            Element* next_element = render_child->GetJSRef();
            return LynxValue::MakeObject(next_element);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue> Element::GetChildNodes() {
        lynx::RenderObject* render_child =
                static_cast<lynx::RenderObject*>(render_object_->FirstChild());
        LynxArray* array = lynx_new LynxArray();
        if(render_object_->GetChildCount() == 0) {
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

            array->Push(LynxValue::MakeObject(child_element).Release());
            render_child = static_cast<lynx::RenderObject*>(render_child->Next());
        }
        return LynxValue::MakeValueScoped(array);
    }

    void Element::SetScrollTop(base::ScopedPtr<jscore::LynxValue> value) {
        int scroll_top = value->data_.i;
        render_object_->SetScrollTop(scroll_top);
    }

    void Element::SetScrollLeft(base::ScopedPtr<jscore::LynxValue> value) {
        int scroll_left = value->data_.i;
        render_object_->SetScrollLeft(scroll_left);
    }

    base::ScopedPtr<LynxValue> Element::Animate(base::ScopedPtr<LynxArray>& array) {
        if (array.Get() != NULL && array->Size() == 2
                && array->Get(0)->type_ == LynxValue::Type::VALUE_LYNX_ARRAY
                && array->Get(1)->type_ == LynxValue::Type::VALUE_LYNX_MAP) {
            base::ScopedPtr<LynxArray> keyframes =
                    LynxValue::MakeArrayScoped(array->Release(0).Release()->data_.lynx_array);
            base::ScopedPtr<LynxMap> options =
                    LynxValue::MakeMapScoped(array->Release(1).Release()->data_.lynx_map);

            auto animation = lynx_new Animation(context_, render_object_->Animate(keyframes, options));
            animation->set_render_object(render_object_->weak_ptr());
            return LynxValue::MakeObject(animation);
        }
        return base::ScopedPtr<LynxValue>();
    }

    void Element::SetAllExtraMethod(ClassTemplate* class_template) {
        int type = render_object_->render_object_type();
        auto methods =  s_rpc_methods.find(type);
        if(methods != s_rpc_methods.end()){
           element_methods = methods->second;
           std::map<std::string,int>::iterator method_itr;
           for (method_itr = element_methods.begin();
                method_itr != element_methods.end(); ++method_itr) {
               class_template->RegisterMethodCallback(method_itr->first, &ExtraCallback);
            }
        }
    }
}
