// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef render_tree_host_impl_ios_h
#define render_tree_host_impl_ios_h

#include "render/render_tree_host_impl.h"

#include "base/lynx_render_tree_host_impl.h"

namespace lynx {
    class RenderTreeHostImplIOS : public RenderTreeHostImpl, public RenderTreeHostImpl::VSyncListener {
        
    public:
        RenderTreeHostImplIOS(jscore::ThreadManager* thread_manager, RenderTreeHost* host, RenderObjectImpl* root);
        virtual ~RenderTreeHostImplIOS();
        
        
        virtual void OnVSync();
        
        virtual void BeginFrame();
        virtual void PrepareCommit();
        
        LynxRenderTreeHostImpl* ios() {
            return ios_;
        }
        
    private:
        LynxRenderTreeHostImpl *ios_;
    };
}

#endif /* render_tree_host_impl_ios_h */
