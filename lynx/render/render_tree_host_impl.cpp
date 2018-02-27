// Copyright 2017 The Lynx Authors. All rights reserved.
#include "render/render_tree_host_impl.h"

#include "render/impl/render_object_impl.h"
#include "render/render_object.h"
#include "render/render_tree_host.h"

#include "base/trace_event/trace_event_common.h"
namespace lynx {

RenderTreeHostImpl::RenderTreeHostImpl(jscore::ThreadManager* thread_manager,
                                       RenderTreeHost* host,
                                       RenderObjectImpl* root)
    : render_state_(RENDER_STATE_NONE),
      thread_manager_(thread_manager),
      render_tree_host_(host),
      render_root_(root),
      is_parse_finished_(false),
      is_first_layouted_(false) {}

RenderTreeHostImpl::~RenderTreeHostImpl() {}

void RenderTreeHostImpl::NextAction() {
  if (render_state_ == RENDER_STATE_NONE) {
    render_state_ = RENDER_STATE_BEGIN_FRAME;
  } else if (render_state_ == RENDER_STATE_BEGIN_FRAME_COMPLETE) {
    render_state_ = RENDER_STATE_COMMIT;
  } else if (render_state_ == RENDER_STATE_COMMIT_COMPLETE) {
    render_state_ = RENDER_STATE_NONE;
  } else if (render_state_ == RENDER_STATE_COMPLETE) {
    render_state_ = RENDER_STATE_NONE;
  }
}

void RenderTreeHostImpl::DoRenderAction() {
  NextAction();
  switch (render_state_) {
    case RENDER_STATE_BEGIN_FRAME:
      BeginFrame();
      break;
    case RENDER_STATE_COMMIT:
      Commit();
      break;
    default:
      break;
  }
}

bool RenderTreeHostImpl::FirstLayout() {
  if (is_parse_finished_ && !is_first_layouted_) {
    render_tree_host_->ForceLayout(0, 0, viewport_.width_, viewport_.height_);
    render_tree_host_->ForceFlushCommands();
    base::ScopedRefPtr<RenderTreeHost> ref(render_tree_host_);
    //thread_manager_->RunOnJSThread(base::Bind(&RenderTreeHost::TreeSync, ref));
    is_first_layouted_ = true;
  }
  return is_first_layouted_;
}

void RenderTreeHostImpl::UpdateViewport(int width, int height) {
  viewport_.width_ = width;
  viewport_.height_ = height;

  BeginFrame();
}

void RenderTreeHostImpl::BeginFrame() {
  if (!FirstLayout()) {
    return;
  }

  RenderTreeHost::BeginFrameData data;
  data.viewport_ = viewport_;
  data.client_ = this;
  base::ScopedRefPtr<RenderTreeHost> ref(render_tree_host_);
  thread_manager_->RunOnJSThread(
      base::Bind(&RenderTreeHost::DoBeginFrame, ref, data));
}

void RenderTreeHostImpl::NotifyBeginFrameComplete() {
  base::ScopedRefPtr<RenderTreeHostImpl> ref(this);
  thread_manager_->RunOnUIThread(
      base::Bind(&RenderTreeHostImpl::BeginFrameComplete, ref));
}

void RenderTreeHostImpl::BeginFrameComplete() {
  // next action do commit
  // render_state_ = RENDER_STATE_BEGIN_FRAME_COMPLETE;

  // next action do noting wait next frame
  render_state_ = RENDER_STATE_COMPLETE;
  DoRenderAction();
}

void RenderTreeHostImpl::Commit() {
  render_tree_host_->DoCommit();
  render_state_ = RENDER_STATE_COMMIT_COMPLETE;
  NextAction();
}

}  // namespace lynx
