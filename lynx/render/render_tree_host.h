// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_RENDER_TREE_HOST_H_
#define LYNX_RENDER_RENDER_TREE_HOST_H_

#include "render/impl/command_collector.h"
#include "render/render_object.h"
#include "render/render_tree_host_client.h"
#include "runtime/js_context.h"
#include "runtime/thread_manager.h"

namespace lynx {
class RenderTreeHostImpl;
class RenderObject;
class RenderTreeHost : public base::RefCountPtr<RenderTreeHost> {
 public:
  struct BeginFrameData {
    base::Position viewport_;
    RenderTreeHostClient* client_;
  };
  /*
      class RenderTreeHostClient {
      public:
          virtual void LoadUrl(const std::string& url, int type) = 0;
          virtual void NotifyTreeSyncCompleted() = 0;
      };
  */
  RenderTreeHost(jscore::JSContext* context,
                 jscore::ThreadManager* thread_manager,
                 RenderObject* root);
  ~RenderTreeHost();

  void UpdateRenderObject(RenderCommand* command);

  void DoBeginFrame(const BeginFrameData& data);
  void DoCommit();

  void ForceLayout(int left, int top, int right, int bottom);
  void ForceFlushCommands();
  void TreeSync();
  void RendererSync(RenderObject* renderer);
  RenderObject* GetElementById(const std::string& id) {
    return renderer_id_map_[id];
  }

  void SetElementId(RenderObject* renderer, const std::string& id) {
    renderer_id_map_[id] = renderer;
  }

  RenderTreeHostImpl* host_impl() { return render_tree_host_impl_.Get(); }

  void SetRenderRoot(RenderObject* root);

  RenderObject* render_root() { return render_root_; }

  jscore::ThreadManager* thread_manager() { return thread_manager_; }

  const base::Position& viewport() { return viewport_; }

  jscore::JSContext* context() { return context_; }

  inline std::string page_location() { return page_location_; }

  void set_page_location(const std::string& page_location) {
    page_location_ = page_location;
  }

 private:
  void PrepareCommit(const BeginFrameData& data);

  base::WeakPtr<RenderTreeHost> weak_ptr_;
  RenderCommandCollector collector_;
  jscore::JSContext* context_;
  jscore::ThreadManager* thread_manager_;
  RenderObject* render_root_;
  base::ScopedRefPtr<RenderTreeHostImpl> render_tree_host_impl_;
  base::Position viewport_;
  bool did_first_layout_;
  std::map<std::string, RenderObject*> renderer_id_map_;
  std::string page_location_;
};
}  // namespace lynx

#endif  // LYNX_RENDER_RENDER_TREE_HOST_H_
