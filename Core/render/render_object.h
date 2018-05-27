// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_RENDER_OBJECT_H_
#define LYNX_RENDER_RENDER_OBJECT_H_

#include <string>

#include "runtime/canvas_cmd.h"

#include "base/position.h"
#include "base/scoped_ptr.h"
#include "base/scoped_ptr_map.h"
#include "base/scoped_vector.h"
#include "base/size.h"
#include "layout/css_style.h"
#include "layout/layout_object.h"
#include "render/event_target.h"
#include "render/render_object_type.h"
#include "runtime/base/lynx_function.h"
#include "runtime/base/lynx_value.h"
#include "runtime/thread_manager.h"

namespace lynx {
class TouchEvent;
class RenderObjectImpl;
class RenderTreeHost;
class RenderObject : public LayoutObject, public EventTarget {
 public:

 RenderObject(const char* tag_name,
              RenderObjectType type,
              uint64_t id,
              RenderObjectImpl* impl,
              RenderTreeHost* host);
 virtual ~RenderObject();

 virtual void SetStyle(const std::string& key,
                       const std::string& value) override;
  virtual void FlushStyle();


 // impl virtual method in ConatinerNode
 virtual void InsertChild(ContainerNode* child, int index) override;
 virtual void RemoveChild(ContainerNode* child) override;

 // impl virtual method in EventTarget
 virtual void RegisterEvent(const std::string& event,
                            RegisterEventType type) override;

 // override virtual method in LayoutObject
 base::Size Measure(int width_descriptor, int height_descriptor) override final;
 void Layout(int left, int top, int right, int bottom) override;

 virtual void SetText(const std::string& text);

 virtual const std::string& GetText() { return text_; }

 RenderObject* NextSibling();

 void AppendChild(RenderObject* child);

 RenderObject* RemoveChildByIndex(int index);

 virtual void InsertBefore(RenderObject* child, RenderObject* reference);

 // operator for attributes
 typedef std::map<std::string, std::string> Attributes;
 typedef std::map<std::string, std::string> Styles;

 virtual void SetAttribute(const std::string& key, const std::string& value);
 bool HasAttribute(const std::string& key);
 void RemoveAttribute(const std::string& key);
 const Attributes& attributes() { return attributes_; }
 const Styles& styles() { return styles_; }

 // Sync attributes from element impl
 void UpdateData(int key, base::ScopedPtr<jscore::LynxValue> value);
 void SetData(int key, base::ScopedPtr<jscore::LynxValue> value);

 inline std::string& tag_name() { return tag_name_; }

 inline void set_scroll_height(int scroll_height) {
   scroll_height_ = scroll_height;
  }

  inline int scroll_height() { return scroll_height_; }

  inline void set_scroll_width(int scroll_width) {
    scroll_width_ = scroll_width;
  }

  inline int scroll_width() { return scroll_width_; }

  void SetScrollLeft(int scroll_left);

  inline int scroll_left() { return scroll_left_; }

  void SetScrollTop(int scroll_top);

  virtual int GetNodeType() {return 1;}
  inline int scroll_top() { return scroll_top_; }

  std::string Animate(
      base::ScopedPtr<jscore::LynxArray>& keyframes,
      base::ScopedPtr<jscore::LynxMap>& options);
  void CancelAnimation(const std::string& id);

  RenderObjectImpl* impl() { return impl_.Get(); }

  const RenderObject* Parent() {
    RenderObject* parent = static_cast<RenderObject*>(parent_);
    if (parent && parent->IsPrivate()) {
      parent = static_cast<RenderObject*>(parent->parent_);
    }
    return parent;
  }

  const RenderObject* Get(int index) {
    return static_cast<RenderObject*>(Find(index));
  }

  const RenderObjectType render_object_type() { return render_object_type_; }
  const bool IsCanvas() { return render_object_type_ == LYNX_CANVAS; }

  const bool IsInvisible() { return render_object_type_ == LYNX_LAYOUT_VIEW; }

  const bool IsPrivate() { return render_object_type_ == LYNX_CELLVIEW; }

  RenderTreeHost* render_tree_host() { return render_tree_host_; }

  void SetJSRef(jscore::Element* js_ref) { SetTarget(js_ref); }

  jscore::Element* GetJSRef() { return static_cast<jscore::Element*>(GetTarget()); }

  uint64_t id() { return id_; }

  base::WeakPtr<RenderObject>& weak_ptr() { return weak_ptr_; }

  void ReceiveCanvasRenderCmd(base::ScopedPtr<base::CanvasRenderCommand>& cmd);

  enum RENDER_OBJECT_ATTRS {
    SCROLL_TOP,
    SCROLL_LEFT,
    GET_TEXT,
    TEXT_LAYOUT,
    CANVAS_DRAW,
    CANVAS_APPEND,
    CANVAS_IMAGE_DATA
  };

 protected:
  void GetVisibleChildren(RenderObject* renderer,
                          std::vector<RenderObject*>& visible_chidren);
  int GetVisibleChildrenLength(RenderObject* renderer);
  void RecalculateLayoutPosition(base::Position& position);
  void HandleFixedStyle();
  void AddFixedChildIfHave(RenderObject* child);
  void RemoveFixedChildIfHave(RenderObject* removed);
  void AddFixedChild(RenderObject* fixed_child);
  void RemoveFixedChild(RenderObject* fixed_child);

  std::string tag_name_;
  uint64_t id_;

  int scroll_height_;
  int scroll_width_;
  int scroll_top_;
  int scroll_left_;

  std::string text_;

  Attributes attributes_;
  Styles styles_;

  bool is_fixed_;
  std::vector<RenderObject*> fixed_children_;

  base::ScopedPtr<jscore::LynxArray> canvas_cmds_;

  RenderObjectType render_object_type_;

  base::ScopedRefPtr<RenderObjectImpl> impl_;

  RenderTreeHost* render_tree_host_;

  base::WeakPtr<RenderObject> weak_ptr_;

  DISALLOW_COPY_AND_ASSIGN(RenderObject);
};

}  // namespace lynx

#endif  // LYNX_RENDER_RENDER_OBJECT_H_
