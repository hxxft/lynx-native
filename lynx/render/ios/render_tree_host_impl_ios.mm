// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/ios/render_tree_host_impl_ios.h"
#include "render/ios/render_object_impl_ios.h"

namespace lynx {
    
    RenderTreeHostImplIOS::RenderTreeHostImplIOS(jscore::ThreadManager* thread_manager, RenderTreeHost* host, RenderObjectImpl* root) : RenderTreeHostImpl(thread_manager, host, root) {
        RenderObjectImplIOS * rootIOS = static_cast<RenderObjectImplIOS *>(root);
        ios_ = [[LynxRenderTreeHostImpl alloc] initWithRenderTreeHost:this
                                                  andRenderObjectImpl:rootIOS];
    }
    
    RenderTreeHostImplIOS::~RenderTreeHostImplIOS() {
        ios_ = nil;
    }

    void RenderTreeHostImplIOS::OnVSync() {
        DoRenderAction();
    }
    
    void RenderTreeHostImplIOS::BeginFrame() {
        [ios_ beginFrame];
        RenderTreeHostImpl::BeginFrame();
    }
    
    void RenderTreeHostImplIOS::PrepareCommit() {
        [ios_ prepareCommit];
    }
    
}