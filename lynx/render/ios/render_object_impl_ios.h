// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_OBJECT_PROXY_IOS_H_
#define LYNX_RENDER_OBJECT_PROXY_IOS_H_

#import "LynxRenderObjectImpl.h"

#include <string>
#include <unordered_set>
#include "render/impl/render_object_impl.h"
#include "runtime/base/lynx_value.h"


namespace lynx {
    class RenderObjectImplIOS : public RenderObjectImpl {
    public:
        
        RenderObjectImplIOS(jscore::ThreadManager* manager, RenderObjectType type);
        virtual ~RenderObjectImplIOS();
        
        void DispatchEvent(const std::string& event, NSMutableArray *array);
        void UpdateData(int attr, id value);
        
        LynxRenderObjectImpl* ios() {
            return ios_;
        }
        
    private:
        virtual void UpdateStyle(const CSSStyle& style);
        virtual void SetPosition(const base::Position& position);
        virtual void SetSize(const base::Size& size);
        virtual void InsertChild(RenderObjectImpl* child, int index);
        virtual void RemoveChild(RenderObjectImpl* child);
        virtual void SetText(const std::string& text);
        virtual void SetAttribute(const std::string& key, const std::string& value);
        virtual void RequestLayout();
        virtual void AddEventListener(const std::string& event);
        virtual void RemoveEventListener(const std::string& event);
        virtual void SetData(int attr, base::ScopedPtr<jscore::LynxValue> value);
        
        void DispatchEventOnJSThread(const std::string& event, base::ScopedPtr<jscore::LynxArray> array);
        void UpdateDataOnJSThread(int attr, base::ScopedPtr<jscore::LynxValue> value);
        
    private:
        LynxRenderObjectImpl* ios_;
    };

}

#endif /* LYNX_RENDER_OBJECT_PROXY_IOS_H_ */
