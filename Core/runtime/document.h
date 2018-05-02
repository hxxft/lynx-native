// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef ANDROID_DOCUMENT_H
#define ANDROID_DOCUMENT_H

#include "runtime/base/lynx_object_template.h"

namespace jscore {
    class JSContext;
    class Element;

    class Document : public LynxObjectTemplate {
    public:
        Document(JSContext* context);
        virtual ~Document();

        Element* CreateElement(std::string& tag_name);
        Element* CreateTextNode(std::string& text);
        Element* GetElementById(std::string& id);
        Element* QuerySelector(std::string &element);

    private:

        JSContext* context_;

        static base::ScopedPtr<LynxValue>
        CreateElementCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        CreateDomCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        CreateTextNodeCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        AddEventListenerCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        RemoveEventListenerCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        DispatchEventCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        CreateEventCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        GetElementByIdCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);
        static base::ScopedPtr<LynxValue>
        QuerySelectorCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);

        static base::ScopedPtr<LynxValue> GetDomainCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetCookieCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetReadyStateCallback(LynxObjectTemplate* object);


        static void SetDomainCallback(LynxObjectTemplate* object,
                                    base::ScopedPtr<jscore::LynxValue> value);
        static void SetCookieCallback(LynxObjectTemplate* object,
                                    base::ScopedPtr<jscore::LynxValue> value);
        static void SetReadyStateCallback(LynxObjectTemplate* object,
                                        base::ScopedPtr<jscore::LynxValue> value);
        static void SetOnTouchStartCallback(LynxObjectTemplate* object,
                                        base::ScopedPtr<jscore::LynxValue> value);
        static void SetOnTouchEndCallback(LynxObjectTemplate* object,
                                        base::ScopedPtr<jscore::LynxValue> value);
        static void SetOnTouchMoveCallback(LynxObjectTemplate* object,
                                        base::ScopedPtr<jscore::LynxValue> value);

    };
}

#endif //ANDROID_DOCUMENT_H
