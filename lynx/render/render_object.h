// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_RENDER_OBJECT_H_
#define LYNX_RENDER_RENDER_OBJECT_H_

#include <string>

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

  // impl virtual method in LayoutObject
  base::Size Measure(int width_descriptor, int height_descriptor);
  void Layout(int left, int top, int right, int bottom);
  // Subclasses should override onMeasure(int, int) to provide
  // a measurements of their content.
  virtual base::Size OnMeasure(int width_descriptor, int height_descriptor);
  // Called from layout when this node should assign a position to each of its children.
  virtual void OnLayout(int left, int top, int right, int bottom);

  virtual void SetStyle(const std::string& key, const std::string& value);

  void FlushStyle();

  // impl virtual method in ConatinerNode
  virtual void InsertChild(ContainerNode* child, int index);
  virtual void RemoveChild(ContainerNode* child);

  // impl virtual method in EventTarget
  virtual void RegisterEvent(const std::string& event, RegisterEventType type);

  virtual void SetText(const std::string& text);
  const std::string& GetText() { return text_; }

  RenderObject* NextSibling();

  void AppendChild(RenderObject* child);

  RenderObject* RemoveChildByIndex(int index);

  virtual void InsertBefore(RenderObject* child, RenderObject* reference);

  // operator for attributes
  typedef std::map<std::string, std::string> Attributes;

  virtual void SetAttribute(const std::string& key, const std::string& value);
  bool HasAttribute(const std::string& key);
  void RemoveAttribute(const std::string& key);
  const Attributes& attributes() { return attributes_; }

  // Sync attributes from element impl
  void UpdateData(int key, base::ScopedPtr<jscore::LynxValue> value);
  void SetData(int key, base::ScopedPtr<jscore::LynxValue> value);

  inline std::string& tag_name() { return tag_name_; }

  inline void set_offset_top(int offset_top) { offset_top_ = offset_top; }

  inline void set_offset_left(int offset_left) { offset_left_ = offset_left; }

  inline int offset_top() { return offset_top_; }

  inline int offset_left() { return offset_left_; }

  inline int offset_width() { return offset_width_; }

  inline int offset_height() { return offset_height_; }

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

  inline int scroll_top() { return scroll_top_; }

  inline void SetForceScrollAnimate(bool animate);

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

  const bool IsInvisible() { return render_object_type_ == LYNX_LAYOUT_VIEW; }

  const bool IsPrivate() { return render_object_type_ == LYNX_CELLVIEW; }

  void SetHost(RenderTreeHost* host) { render_tree_host_ = host; }

  RenderTreeHost* render_tree_host() { return render_tree_host_; }

  void SetJSRef(void* js_ref) { SetTarget(js_ref); }

  void* GetJSRef() { return GetTarget(); }

  uint64_t id() { return id_; }

  enum RENDER_OBJECT_ATTRS {
    SCROLL_TOP,
    SCROLL_LEFT,
    GET_TEXT,
    TEXT_LAYOUT,
    ANIMATE_PROPS,
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

  int offset_top_;
  int offset_left_;
  int offset_width_;
  int offset_height_;

  int scroll_height_;
  int scroll_width_;
  int scroll_top_;
  int scroll_left_;

  std::string text_;

  Attributes attributes_;

  bool is_fixed_;
  std::vector<RenderObject*> fixed_children_;

  RenderObjectType render_object_type_;

  base::ScopedRefPtr<RenderObjectImpl> impl_;

  RenderTreeHost* render_tree_host_;

  base::WeakPtr<RenderObject> weak_ptr_;

  DISALLOW_COPY_AND_ASSIGN(RenderObject);
};

}  // namespace lynx

#endif  // LYNX_RENDER_RENDER_OBJECT_H_
