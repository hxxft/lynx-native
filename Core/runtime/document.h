// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef ANDROID_DOCUMENT_H
#define ANDROID_DOCUMENT_H

#include "runtime/base/lynx_object.h"

namespace jscore {
    class JSContext;
    class Element;

    class Document : public LynxObject {
    public:
        Document(JSContext* context);
        virtual ~Document();

        Element* CreateElement(std::string& tag_name);
        Element* CreateTextNode(std::string& text);
        Element* GetElementById(std::string& id);
        Element* QuerySelector(std::string &element);

        base::ScopedPtr<LynxValue> CreateElement(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> CreateDom(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> CreateTextNode(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> AddEventListener(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> RemoveEventListener(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> DispatchEvent(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> CreateEvent(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> GetElementById(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> QuerySelector(base::ScopedPtr<LynxArray>& array);

        base::ScopedPtr<LynxValue> GetDomain();
        base::ScopedPtr<LynxValue> GetCookie();
        base::ScopedPtr<LynxValue> GetReadyState();
        base::ScopedPtr<LynxValue> GetBody();

        void SetDomain(base::ScopedPtr<jscore::LynxValue> value);
        void SetCookie(base::ScopedPtr<jscore::LynxValue> value);
        void SetReadyState(base::ScopedPtr<jscore::LynxValue> value);
        void SetOnTouchStart(base::ScopedPtr<jscore::LynxValue> value);
        void SetOnTouchEnd(base::ScopedPtr<jscore::LynxValue> value);
        void SetOnTouchMove(base::ScopedPtr<jscore::LynxValue> value);

    private:
        Element* body_;
    };
}

#endif //ANDROID_DOCUMENT_H
