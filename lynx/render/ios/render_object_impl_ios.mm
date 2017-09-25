// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/ios/render_object_impl_ios.h"
#include "render/render_object.h"
#include "base/ios/common.h"
#include "base/ios/oc_helper.h"

using namespace base;

namespace lynx {

    RenderObjectImplIOS::RenderObjectImplIOS(jscore::ThreadManager* manager,RenderObjectType type) : RenderObjectImpl(manager, type){
        ios_ = [[LynxRenderObjectImpl alloc] initWithRenderObjectImpl:this withType:type];
    }
    
    RenderObjectImplIOS::~RenderObjectImplIOS() {
        ios_ = nil;
    }

    void RenderObjectImplIOS::SetPosition(const Position& position) {
        [ios_ setPosition:position];
    }

    void RenderObjectImplIOS::SetSize(const base::Size& size) {
        [ios_ setSize:size];
    }

    void RenderObjectImplIOS::UpdateStyle(const CSSStyle& style) {
        [ios_ updateStyle:style];
    }

    void RenderObjectImplIOS::SetText(const std::string& text) {
        [ios_ setText:[[NSString alloc] initWithUTF8String:text.c_str()]];
    }

    void RenderObjectImplIOS::SetAttribute(const std::string& key, const std::string& value) {
        if (!key.empty()) {
            [ios_ setAttribute:[[NSString alloc] initWithUTF8String:value.c_str()]
                        forKey:[[NSString alloc] initWithUTF8String:key.c_str()]];
        }
    }

    void RenderObjectImplIOS::RequestLayout() {
        [ios_ requestLayout];
    }

    void RenderObjectImplIOS::InsertChild(RenderObjectImpl* child, int index) {
        RenderObjectImplIOS* childROLI = static_cast<RenderObjectImplIOS *>(child);
        [ios_ insertChild:childROLI->ios_ atIndex:index];
    }
    
    void RenderObjectImplIOS::RemoveChild(RenderObjectImpl* child) {
        RenderObjectImplIOS* childROLI = static_cast<RenderObjectImplIOS *>(child);
        [ios_ removeChild:childROLI->ios_];
    }
    
    void RenderObjectImplIOS::AddEventListener(const std::string& event) {
        if (event.empty()) {
            return;
        }
        [ios_ addEventListener:[[NSString alloc] initWithUTF8String:event.c_str()]];
    }
    
    void RenderObjectImplIOS::RemoveEventListener(const std::string& event) {
        if (event.empty()) return;
        [ios_ removeEventListener:[[NSString alloc] initWithUTF8String:event.c_str()]];
    }
    
    void RenderObjectImplIOS::SetData(int attr, base::ScopedPtr<jscore::LynxValue> value) {
        id data = base::ios::OCHelper::ConvertToOCValue(value.Get());
        LynxRenderObjectAttr key = (LynxRenderObjectAttr) attr;
        [ios_ setData:data withKey:key];
    }
    
    void RenderObjectImplIOS::DispatchEvent(const std::string& event, NSMutableArray *array) {
        if (!array) {
            @throw [[NSException alloc]
                    initWithName:@"抛出错误" reason:@"array should not be null !" userInfo:nil];
        }
        base::ScopedRefPtr<RenderObjectImplIOS> ref(this);
        base::ScopedPtr<jscore::LynxArray> param(base::ios::OCHelper::ConvertToLynxArray(array));
        thread_manager_->RunOnJSThread(base::Bind(&RenderObjectImplIOS::DispatchEventOnJSThread, ref, event, param));
    }
    
    void RenderObjectImplIOS::DispatchEventOnJSThread(const std::string& event, base::ScopedPtr<jscore::LynxArray> array) {
        if (render_object_weak_ptr_.IsValid()) {
            render_object_weak_ptr_->DispatchEvent(event, array);
        }
    }
    
    void RenderObjectImplIOS::UpdateData(int attr, id value) {
        base::ScopedPtr<jscore::LynxValue> value_transformed(base::ios::OCHelper::ConvertToLynxValue(value));
        base::ScopedRefPtr<RenderObjectImplIOS> ref(this);
        thread_manager_->RunOnJSThread(base::Bind(&RenderObjectImplIOS::UpdateDataOnJSThread, ref, attr, value_transformed));
    }
    
    void RenderObjectImplIOS::UpdateDataOnJSThread(int attr, base::ScopedPtr<jscore::LynxValue> value) {
        if (render_object_weak_ptr_.IsValid()) {
            render_object_weak_ptr_->UpdateData(attr, value);
        }
    }
    
}
