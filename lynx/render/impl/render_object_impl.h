// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_IMPL_RENDER_OBJECT_IMPL_H_
#define LYNX_RENDER_IMPL_RENDER_OBJECT_IMPL_H_

#include <string>

#include "base/position.h"
#include "base/size.h"
#include "base/weak_ptr.h"
#include "render/render_object_type.h"
#include "layout/css_style.h"
#include "runtime/thread_manager.h"
#include "runtime/base/lynx_object.h"
#include "runtime/base/lynx_holder.h"

namespace lynx {
class RenderObject;
class RenderObjectImpl : public base::RefCountPtr<RenderObjectImpl>, public jscore::LynxHolder {
 public:
    RenderObjectImpl(jscore::ThreadManager* manager, RenderObjectType type);

    virtual ~RenderObjectImpl();

    RenderObjectType render_object_type() {
        return render_object_type_;
    }

    jscore::ThreadManager* thread_manager() {
        return thread_manager_;
    }

    void SetRenderObjectWeakRef(
        base::WeakPtr<RenderObject>& weak_ptr) {
        render_object_weak_ptr_ = weak_ptr;
    }

    base::ScopedPtr<jscore::LynxValue> GetLynxValue();

    static RenderObjectImpl* Create(
        jscore::ThreadManager* manager,
        RenderObjectType type);

    virtual void UpdateStyle(const CSSStyle& style) = 0;
    virtual void SetPosition(const base::Position& position) = 0;
    virtual void SetSize(const base::Size& size) = 0;
    virtual void InsertChild(RenderObjectImpl* child, int index) = 0;
    virtual void RemoveChild(RenderObjectImpl* child) = 0;
    virtual void SetText(const std::string& text) = 0;
    virtual void SetAttribute(const std::string& key,
                                const std::string& value) = 0;
    virtual void RequestLayout() = 0;
    virtual void AddEventListener(const std::string& event) = 0;
    virtual void RemoveEventListener(const std::string& event) = 0;
    virtual void SetData(int key, base::ScopedPtr<jscore::LynxValue> value) = 0;
    virtual void Animate(base::ScopedPtr<jscore::LynxArray> &keyframes,
                         base::ScopedPtr<jscore::LynxObject> &options) = 0;
    virtual void CancelAnimation(const std::string &id) = 0;
    virtual base::ScopedPtr<jscore::LynxObject> GetImagePixel(int x, int y, int w,int h) = 0;

 protected:
    jscore::ThreadManager* thread_manager_;
    RenderObjectType render_object_type_;
    base::WeakPtr<RenderObject> render_object_weak_ptr_;

 private:
    DISALLOW_COPY_AND_ASSIGN(RenderObjectImpl);
};
}  // namespace lynx

#endif  // LYNX_RENDER_IMPL_RENDER_OBJECT_IMPL_H_
