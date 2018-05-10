// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_GLOBAL_H_
#define LYNX_RUNTIME_GLOBAL_H_

#include "runtime/base/lynx_object.h"

namespace jscore {
    class Console;
    class Navigator;
    class Screen;
    class Loader;
    class Document;

    class Global : public LynxObject {
    public:
        Global(JSContext* context);
        virtual ~Global();
        void OnJSObjectAttached();
        inline Console* console() { return console_; }
        inline Navigator* navigator() { return navigator_; }
        inline Screen* screen() { return screen_; }
        inline Loader* loader() { return loader_; }
        inline Document* document() { return document_; }

        base::ScopedPtr<LynxValue> SetTimeout(base::ScopedPtr<LynxArray> &array);
        base::ScopedPtr<LynxValue> SetInterval(base::ScopedPtr<LynxArray> &array);
        base::ScopedPtr<LynxValue> ClearTimeout(base::ScopedPtr<LynxArray> &array);
        base::ScopedPtr<LynxValue> ClearInterval(base::ScopedPtr<LynxArray> &array);

        base::ScopedPtr<LynxValue> GetDevicePixelRatio();
        base::ScopedPtr<LynxValue> GetConsole();
        base::ScopedPtr<LynxValue> GetNavigator();
        base::ScopedPtr<LynxValue> GetScreen();
        base::ScopedPtr<LynxValue> GetLoader();
        base::ScopedPtr<LynxValue> GetDocument();

    private:
        Console* console_;
        Navigator* navigator_;
        Screen* screen_;
        Loader* loader_;
        Document* document_;

    };
}

#endif //LYNX_RUNTIME_GLOBAL_H_
