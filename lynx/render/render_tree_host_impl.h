// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_RENDER_TREE_HOST_IMPL_H_
#define LYNX_RENDER_RENDER_TREE_HOST_IMPL_H_

#include "base/ref_counted_ptr.h"
#include "base/position.h"
#include "render/render_tree_host_client.h"
#include "runtime/thread_manager.h"

namespace lynx {

class RenderTreeHost;
class RenderObjectImpl;
class RenderTreeHostImpl : public RenderTreeHostClient,
    public base::RefCountPtr<RenderTreeHostImpl> {
 public:
    enum RenderState {
        RENDER_STATE_NONE,
        RENDER_STATE_BEGIN_FRAME,
        RENDER_STATE_BEGIN_FRAME_COMPLETE,
        RENDER_STATE_COMMIT,
        RENDER_STATE_COMMIT_COMPLETE,
        RENDER_STATE_COMPLETE,
    };

    class VSyncListener {
     public:
        virtual void OnVSync() = 0;
    };

    RenderTreeHostImpl(jscore::ThreadManager* thread_manager,
                       RenderTreeHost* host,
                       RenderObjectImpl* root);
    virtual ~RenderTreeHostImpl();

    void DoRenderAction();
    void UpdateViewport(int left, int top, int width, int height);

    virtual void BeginFrame();
    virtual void Commit();

    virtual void PrepareCommit() {}

    void NotifyBeginFrameComplete();

    void SetParseFinished() {
        is_parse_finished_ = true;
    }

 private:
    void NextAction();

    void BeginFrameComplete();

    bool FirstLayout();

 protected:
    RenderState render_state_;
    jscore::ThreadManager* thread_manager_;
    RenderTreeHost* render_tree_host_;
    base::ScopedRefPtr<RenderObjectImpl> render_root_;
    base::Position viewport_;

    bool is_parse_finished_;
    bool is_first_layouted_;
};
}  // namespace lynx

#endif  // LYNX_RENDER_RENDER_TREE_HOST_IMPL_H_
