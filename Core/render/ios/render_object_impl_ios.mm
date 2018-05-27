// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/ios/render_object_impl_ios.h"
#include "render/render_object.h"
#include "base/ios/common.h"
#include "base/ios/oc_helper.h"

using namespace base;

namespace lynx {

    RenderObjectImpl* RenderObjectImpl::Create(jscore::ThreadManager *proxy,
                                               RenderObjectType type) {
        return lynx_new RenderObjectImplIOS(proxy, type);
    }
    
    RenderObjectImplIOS::RenderObjectImplIOS(jscore::ThreadManager* manager,RenderObjectType type) : RenderObjectImpl(manager, type){
        renderer_bridge_ = [[RenderObjectImplBridge alloc] initWithRenderObjectImpl:this withType:type];
    }
    
    RenderObjectImplIOS::~RenderObjectImplIOS() {
        renderer_bridge_ = nil;
    }

    void RenderObjectImplIOS::SetPosition(const Position& position) {
        [renderer_bridge_ setPosition:position];
    }

    void RenderObjectImplIOS::SetSize(const base::Size& size) {
        [renderer_bridge_ setSize:size];
    }

    void RenderObjectImplIOS::UpdateStyle(const CSSStyle& style) {
        [renderer_bridge_ updateStyle:style];
    }

    void RenderObjectImplIOS::SetText(const std::string& text) {
        [renderer_bridge_ setText:[[NSString alloc] initWithUTF8String:text.c_str()]];
    }

    void RenderObjectImplIOS::SetAttribute(const std::string& key, const std::string& value) {
        if (!key.empty()) {
            [renderer_bridge_ setAttribute:[[NSString alloc] initWithUTF8String:value.c_str()]
                        forKey:[[NSString alloc] initWithUTF8String:key.c_str()]];
        }
    }

    void RenderObjectImplIOS::RequestLayout() {
        [renderer_bridge_ requestLayout];
    }

    void RenderObjectImplIOS::InsertChild(RenderObjectImpl* child, int index) {
        RenderObjectImplIOS* childROLI = static_cast<RenderObjectImplIOS *>(child);
        [renderer_bridge_ insertChild:childROLI->renderer_bridge_ atIndex:index];
    }
    
    void RenderObjectImplIOS::RemoveChild(RenderObjectImpl* child) {
        RenderObjectImplIOS* childROLI = static_cast<RenderObjectImplIOS *>(child);
        [renderer_bridge_ removeChild:childROLI->renderer_bridge_];
    }
    
    void RenderObjectImplIOS::AddEventListener(const std::string& event) {
        if (event.empty()) {
            return;
        }
        [renderer_bridge_ addEventListener:[[NSString alloc] initWithUTF8String:event.c_str()]];
    }
    
    void RenderObjectImplIOS::RemoveEventListener(const std::string& event) {
        if (event.empty()) return;
        [renderer_bridge_ removeEventListener:[[NSString alloc] initWithUTF8String:event.c_str()]];
    }
    
    void RenderObjectImplIOS::SetData(int attr, base::ScopedPtr<jscore::LynxValue> value) {
        id data = base::ios::OCHelper::ConvertToOCValue(value.Get());
        LynxRenderObjectAttr key = (LynxRenderObjectAttr) attr;
        [renderer_bridge_ setData:data withKey:key];
    }
    
    void RenderObjectImplIOS::Animate(base::ScopedPtr<jscore::LynxArray> &keyframes,
                                      base::ScopedPtr<jscore::LynxMap> &options) {
        
    }
    
    void RenderObjectImplIOS::CancelAnimation(const std::string &id) {
        
    }
    
    void RenderObjectImplIOS::DispatchEvent(const std::string& event, NSArray *array) {
        if (!array) {
            @throw [[NSException alloc]
                    initWithName:@"dispatch event error" reason:@"array should not be null !" userInfo:nil];
        }
        base::ScopedRefPtr<RenderObjectImplIOS> ref(this);
        base::ScopedPtr<jscore::LynxArray> param(base::ios::OCHelper::ConvertToLynxArray(array));
        //thread_manager_->RunOnJSThread(base::Bind(&RenderObjectImplIOS::DispatchEventOnJSThread, ref, event, param));
        if (render_object_weak_ptr_.IsValid()) {
            render_object_weak_ptr_->DispatchEvent(event, param);
        }
    }
    
    void RenderObjectImplIOS::DispatchEventOnJSThread(const std::string& event, base::ScopedPtr<jscore::LynxArray> array) {
        if (render_object_weak_ptr_.IsValid()) {
            render_object_weak_ptr_->DispatchEvent(event, array);
        }
    }
    
    void RenderObjectImplIOS::UpdateData(int attr, id value) {
        base::ScopedPtr<jscore::LynxValue> value_transformed(base::ios::OCHelper::ConvertToLynxValue(value));
        base::ScopedRefPtr<RenderObjectImplIOS> ref(this);
        //thread_manager_->RunOnJSThread(base::Bind(&RenderObjectImplIOS::UpdateDataOnJSThread, ref, attr, value_transformed));
        
        if (render_object_weak_ptr_.IsValid()) {
            render_object_weak_ptr_->UpdateData(attr, value_transformed);
        }
    }
    
    void RenderObjectImplIOS::UpdateDataOnJSThread(int attr, base::ScopedPtr<jscore::LynxValue> value) {
        if (render_object_weak_ptr_.IsValid()) {
            render_object_weak_ptr_->UpdateData(attr, value);
        }
    }
    
    base::ScopedPtr<jscore::LynxMap> RenderObjectImplIOS::GetImagePixel(int x, int y, int w,int h) {
        return base::ScopedPtr<jscore::LynxMap>();
    }
}
