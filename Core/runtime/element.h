// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_ELEMENT_H_
#define LYNX_RUNTIME_ELEMENT_H_

#include "runtime/base/lynx_object.h"

namespace lynx {
    class RenderObject;
}

namespace jscore {

    class Element : public LynxObject {
    public:
        Element(JSContext* context);
        Element(JSContext* context, lynx::RenderObject* render_object);
        virtual ~Element();

        lynx::RenderObject* render_object() {
            return render_object_.Get();
        }

        std::map<std::string,int> element_methods;

        static std::map<int,std::map<std::string, int>> s_rpc_methods;
        static std::map<std::string, int> s_element_tags;

        base::ScopedPtr<LynxValue> AppendChild(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> AppendChildren(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> InsertChildAtIndex(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> RemoveChildByIndex(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> InsertBefore(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> RemoveChild(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> GetChildByIndex(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> AddEventListener(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> RemoveEventListener(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> SetAttribution(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> SetAttribute(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> HasAttribute(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> RemoveAttribute(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> SetStyle(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> SetText(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> GetText(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> HasChildNodes(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> Animate(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> Extra(const std::string& function,
                                         base::ScopedPtr<LynxArray>& array);

        base::ScopedPtr<LynxValue> GetTagName();
        base::ScopedPtr<LynxValue> GetNodeType();
        base::ScopedPtr<LynxValue> GetParentNode();
        base::ScopedPtr<LynxValue> GetOffsetTop();
        base::ScopedPtr<LynxValue> GetOffsetLeft();
        base::ScopedPtr<LynxValue> GetOffsetWidth();
        base::ScopedPtr<LynxValue> GetOffsetHeight();
        base::ScopedPtr<LynxValue> GetScrollWidth();
        base::ScopedPtr<LynxValue> GetScrollHeight();
        base::ScopedPtr<LynxValue> GetScrollTop();
        base::ScopedPtr<LynxValue> GetScrollLeft();
        base::ScopedPtr<LynxValue> GetTextContent();
        base::ScopedPtr<LynxValue> GetNextSibling();
        base::ScopedPtr<LynxValue> GetClientWidth();
        base::ScopedPtr<LynxValue> GetClientHeight();
        base::ScopedPtr<LynxValue> GetChildNodes();
        base::ScopedPtr<LynxValue> GetFirstChild();

        void SetTextContent(base::ScopedPtr<jscore::LynxValue> value);
        void SetScrollTop(base::ScopedPtr<jscore::LynxValue> value);
        void SetScrollLeft(base::ScopedPtr<jscore::LynxValue> value);

    private:

        base::ScopedPtr<lynx::RenderObject> render_object_;

        void SetAllExtraMethod(ClassTemplate* class_template);
    };
}

#endif //LYNX_RUNTIME_ELEMENT_H_
