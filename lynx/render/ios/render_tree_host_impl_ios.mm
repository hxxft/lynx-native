// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/ios/render_tree_host_impl_ios.h"
#include "render/ios/render_object_impl_ios.h"

namespace lynx {
    RenderTreeHostImpl* RenderTreeHostImpl::Create(jscore::ThreadManager* thread_manager, RenderTreeHost* host, RenderObjectImpl* root) {
        return lynx_new RenderTreeHostImplIOS(thread_manager, host, root);
    }
    
    RenderTreeHostImplIOS::RenderTreeHostImplIOS(jscore::ThreadManager* thread_manager, RenderTreeHost* host, RenderObjectImpl* root) : RenderTreeHostImpl(thread_manager, host, root) {
        RenderObjectImplIOS * rootIOS = static_cast<RenderObjectImplIOS *>(root);
        tree_host_bridge_ = [[RenderTreeHostImplBridge alloc] initWithRenderTreeHost:this
                                                  andRenderObjectImpl:rootIOS];
    }
    
    RenderTreeHostImplIOS::~RenderTreeHostImplIOS() {
        tree_host_bridge_ = nil;
    }

    void RenderTreeHostImplIOS::OnVSync() {
        DoRenderAction();
    }
    
    void RenderTreeHostImplIOS::BeginFrame() {
        [tree_host_bridge_ beginFrame];
        RenderTreeHostImpl::BeginFrame();
    }
    
    void RenderTreeHostImplIOS::PrepareCommit() {
        [tree_host_bridge_ prepareCommit];
    }
    
}
