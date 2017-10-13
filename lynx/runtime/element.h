// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_ELEMENT_H_
#define LYNX_RUNTIME_ELEMENT_H_

#include "runtime/base/lynx_object_template.h"
#include "render/render_object.h"

namespace jscore {
    class JSContext;

    class Element : public LynxObjectTemplate {
    public:
        Element(JSContext* context, lynx::RenderObject* render_object);
        virtual ~Element();

        lynx::RenderObject* render_object() {
            return render_object_.Get();
        }

        JSContext* context() {
            return context_;
        }

        static void ProtectChild(JSContext* context, Element* child);
        static void UnprotectChild(JSContext* context, Element* child);

    private:

        bool is_protect_;
        base::ScopedPtr<lynx::RenderObject> render_object_;
        JSContext* context_;

        static base::ScopedPtr<LynxValue>
        AppendChildCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        AppendChildrenCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        InsertChildAtIndexCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        RemoveChildByIndexCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        InsertBeforeCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        RemoveChildCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        GetChildByIndexCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        AddEventListenerCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        RemoveEventListenerCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        SetAttributionCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        SetAttributeCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        HasAttributeCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        RemoveAttributeCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        SetStyleCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        SetTextCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        GetTextCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        StartCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        StopCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        StopAnimateCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        StartAnimateWithCallbackCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        SetPullViewCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        ClosePullViewCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        HasChildNodesCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);


        static base::ScopedPtr<LynxValue> GetTagNameCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetNodeTypeCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetParentNodeCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetOffsetTopCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetOffsetLeftCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetOffsetWidthCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetOffsetHeightCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetScrollWidthCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetScrollHeightCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetScrollTopCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetScrollLeftCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetTextContentCallback(LynxObjectTemplate* object);
        static void SetTextContentCallback(LynxObjectTemplate* object,
                                                                 base::ScopedPtr<jscore::LynxValue> value);
        static base::ScopedPtr<LynxValue> GetNextSiblingCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetChildNodesCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetFirstChildCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetIndexCallback(LynxObjectTemplate* object);

        static void SetScrollTopCallback(LynxObjectTemplate* object,
                                    base::ScopedPtr<jscore::LynxValue> value);
        static void SetScrollLeftCallback(LynxObjectTemplate* object,
                                    base::ScopedPtr<jscore::LynxValue> value);
        static void SetForceScrollAnimateCallback(LynxObjectTemplate* object,
                                    base::ScopedPtr<jscore::LynxValue> value);
    };
}

#endif //LYNX_RUNTIME_ELEMENT_H_
