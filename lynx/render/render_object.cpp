// Copyright 2017 The Lynx Authors. All rights reserved.


#include <sstream>
#include <vector>

#include "render/impl/render_command.h"
#include "render/impl/render_object_impl.h"
#include "render/render_tree_host.h"
#include "runtime/base/lynx_value.h"
#include "runtime/base/lynx_array.h"
#include "render/render_object.h"

namespace lynx {

RenderObject::RenderObject(const char* tag_name,
                           RenderObjectType type,
                           uint64_t id,
                           RenderObjectImpl* impl,
                           RenderTreeHost* host)
    : tag_name_(tag_name),
      id_(id),
      offset_top_(0),
      offset_left_(0),
      offset_width_(0),
      offset_height_(0),
      scroll_height_(0),
      scroll_width_(0),
      scroll_top_(0),
      scroll_left_(0),
      text_(""),
      canvas_cmds_(0),
      is_fixed_(false),
      render_object_type_(type),
      impl_(impl),
      render_tree_host_(host),
      weak_ptr_(this) {
  if (impl != NULL) {
    impl->SetRenderObjectWeakRef(weak_ptr_);
  }
}

RenderObject::~RenderObject() {
  weak_ptr_.Invalidate();
}

void RenderObject::RegisterEvent(const std::string& event,
                                 RegisterEventType type) {
  RenderCommand* cmd = NULL;
  switch (type) {
    case EVENT_ADD:
      cmd = lynx_new RendererEventUpdateCommand(
          impl(), event, RenderCommand::CMD_ADD_EVENT_LISTENER);
      break;
    case EVENT_REMOVE:
      cmd = lynx_new RendererEventUpdateCommand(
          impl(), event, RenderCommand::CMD_REMOVE_EVENT_LISTENER);
      break;
    default:
      break;
  }
  render_tree_host_->UpdateRenderObject(cmd);
}

void RenderObject::GetVisibleChildren(
    RenderObject* renderer,
    std::vector<RenderObject*>& visible_chidren) {
  if (!renderer->IsInvisible()) {
    visible_chidren.push_back(renderer);
    return;
  }
  RenderObject* child = static_cast<RenderObject*>(renderer->FirstChild());
  while (child) {
    GetVisibleChildren(child, visible_chidren);
    child = static_cast<RenderObject*>(child->Next());
  }
}

int RenderObject::GetVisibleChildrenLength(RenderObject* renderer) {
  int length = 0;
  if (!renderer->IsInvisible()) {
    length++;
  } else {
    RenderObject* child = static_cast<RenderObject*>(renderer->FirstChild());
    while (child) {
      length += GetVisibleChildrenLength(child);
      child = static_cast<RenderObject*>(child->Next());
    }
  }
  return length;
}

RenderObject* RenderObject::NextSibling() {
  RenderObject* next = static_cast<RenderObject*>(Next());

  if (parent_ && static_cast<RenderObject*>(parent_)->IsPrivate()) {
    next = static_cast<RenderObject*>(parent_->Next());
  }

  if (next && next->IsPrivate()) {
    next = static_cast<RenderObject*>(next->Find(0));
  }
  return next;
}

void RenderObject::SetStyle(const std::string& key, const std::string& value) {
  if (!key.empty()) {
    LayoutObject::SetStyle(key, value);
    HandleFixedStyle();
  } else {
    FlushStyle();
  }
}

void RenderObject::InsertChild(ContainerNode* child, int index) {
  if (child == NULL)
    return;
  LayoutObject::InsertChild(child, index);

  RenderObject* renderer = this;

  while (renderer->IsInvisible()) {
    renderer = const_cast<RenderObject*>(renderer->Parent());
    if (renderer == NULL)
      return;
  }

  std::vector<RenderObject*> visible_children;
  GetVisibleChildren(static_cast<RenderObject*>(child), visible_children);

  // Find correct insert index
  int final_insert_index = 0;
  int insert_index = index < 0 ? GetChildCount() - 1 : index;
  for (int i = 0; i < insert_index; ++i) {
    RenderObject* temp = static_cast<RenderObject*>(Find(i));
    if (temp->style_.css_position_type_ == CSSStyleType::CSS_POSITION_FIXED)
      continue;
    final_insert_index += GetVisibleChildrenLength(temp);
  }

  for (int i = 0; i < visible_children.size(); ++i) {
    RenderObject* visible_child = visible_children.at(i);
    RenderCommand* cmd = lynx_new RendererOperatorCommand(
        renderer->impl(), visible_child->impl_.Get(), i + final_insert_index,
        RenderCommand::CMD_ADD_VIEW);
    render_tree_host_->UpdateRenderObject(cmd);
  }

  AddFixedChildIfHave(static_cast<RenderObject*>(child));
}

void RenderObject::RecalculateLayoutPosition(base::Position& position) {
  RenderObject* parent = const_cast<RenderObject*>(Parent());
  while (parent && parent->IsInvisible()) {
    base::Vector2D offset = parent->measured_position_.Offset();
    position.Update(offset);
    parent = const_cast<RenderObject*>(parent->Parent());
  }
}

base::Size RenderObject::Measure(int width, int height) {
    base::Size old_size = measured_size_;
    LayoutObject::Measure(width, height);
    if (old_size.Update(measured_size_.width_, measured_size_.height_) && !IsInvisible()) {
        base::Size size(base::Size::Descriptor::GetSize(measured_size_.width_),
                        base::Size::Descriptor::GetSize(measured_size_.height_));
        RenderCommand* cmd = lynx_new RendererSizeUpdateCommand(impl(),
                                                                size,
                                                                RenderCommand::CMD_SET_SIZE);
        render_tree_host_->UpdateRenderObject(cmd);
    }
    return measured_size_;
}

void RenderObject::Layout(int left, int top, int right, int bottom) {
    offset_top_ = top;
    offset_left_ = left;
    offset_height_ = bottom - top;
    offset_width_ = right - left;

    if (measured_position_.NeedToReset(left, top, right, bottom) && !IsInvisible()) {
        base::Position position(left, top, right, bottom);
        RecalculateLayoutPosition(position);
        RenderCommand* cmd = lynx_new RendererPosUpdateCommand(impl(),
                                                               position,
                                                               RenderCommand::CMD_SET_POSITION);
        render_tree_host_->UpdateRenderObject(cmd);
    }
    LayoutObject::Layout(left, top, right, bottom);
}

void RenderObject::SetText(const std::string& text) {
    text_ = text;
    if (!IsInvisible()) {
        RenderCommand* cmd = lynx_new RendererAttrUpdateCommand(impl(),
                                                                "",
                                                                text,
                                                                RenderCommand::CMD_SET_LABEL_TEXT);
        render_tree_host_->UpdateRenderObject(cmd);
    }
    Dirty();
}

void RenderObject::AppendChild(RenderObject* child) {
  InsertChild(child, -1);
}

RenderObject* RenderObject::RemoveChildByIndex(int index) {
  return static_cast<RenderObject*>(ContainerNode::RemoveChild(index));
}

void RenderObject::InsertBefore(RenderObject* child, RenderObject* reference) {
  if (child == NULL)
    return;
  InsertChild(child, Find(reference));
}

void RenderObject::RemoveChild(ContainerNode* child) {
  if (child == NULL)
    return;

  RemoveFixedChildIfHave(static_cast<RenderObject*>(child));

  ContainerNode::RemoveChild(child);
  Dirty();

  RenderObject* renderer = this;

  while (renderer->IsInvisible()) {
    renderer = const_cast<RenderObject*>(renderer->Parent());
    if (renderer == NULL)
      return;
  }

  std::vector<RenderObject*> visible_children;
  GetVisibleChildren(static_cast<RenderObject*>(child), visible_children);
  std::vector<RenderObject*>::iterator visible_child = visible_children.begin();
  for (; visible_child != visible_children.end(); ++visible_child) {
    RenderCommand* cmd = lynx_new RendererOperatorCommand(
        renderer->impl(), (*visible_child)->impl_.Get(), 0,
        RenderCommand::CMD_REMOVE_VIEW);
    render_tree_host_->UpdateRenderObject(cmd);
  }
}

void RenderObject::SetAttribute(const std::string &key,
                                const std::string &value) {
    if (!IsInvisible()) {
        RenderCommand* cmd = lynx_new RendererAttrUpdateCommand(impl(), key, value, RenderCommand::CMD_SET_ATTR);
        render_tree_host_->UpdateRenderObject(cmd);

        attributes_[key] = value;
    }
}

bool RenderObject::HasAttribute(const std::string& key) {
  return attributes_.find(key) != attributes_.end();
}

void RenderObject::RemoveAttribute(const std::string& key) {
  std::map<std::string, std::string>::iterator iter = attributes_.find(key);
  if (attributes_.find(key) != attributes_.end()) {
    attributes_.erase(iter);
  }
}

void RenderObject::FlushStyle() {
  if (!IsInvisible()) {
    RenderCommand* cmd = lynx_new RendererStyleUpdateCommand(
        impl(), style_, RenderCommand::CMD_SET_STYLE);
    render_tree_host_->UpdateRenderObject(cmd);
  }
  Dirty();
}

void RenderObject::SetScrollLeft(int scroll_left) {
    base::ScopedPtr<jscore::LynxValue> param = jscore::LynxValue::MakeInt(scroll_left);
    SetData(RENDER_OBJECT_ATTRS::SCROLL_LEFT, param);
}

void RenderObject::SetScrollTop(int scroll_top) {
    base::ScopedPtr<jscore::LynxValue> param = jscore::LynxValue::MakeInt(scroll_top);
    SetData(RENDER_OBJECT_ATTRS::SCROLL_TOP, param);
}

void RenderObject::UpdateData(int key,
                              base::ScopedPtr<jscore::LynxValue> value) {
  switch (key) {
    case RENDER_OBJECT_ATTRS::SCROLL_TOP:
      scroll_top_ = value->data_.i;
      break;
    case RENDER_OBJECT_ATTRS::SCROLL_LEFT:
      scroll_left_ = value->data_.i;
      break;
    case RENDER_OBJECT_ATTRS::GET_TEXT:
      text_ = value->data_.i;
      break;
    case RENDER_OBJECT_ATTRS::CANVAS_IMAGE_DATA:

    default:
      break;
  }
}


void RenderObject::SetData(int key, base::ScopedPtr<jscore::LynxValue> value) {
  switch (key) {
    case RENDER_OBJECT_ATTRS::SCROLL_TOP:
      scroll_top_ = value->data_.i;
      break;
    case RENDER_OBJECT_ATTRS::SCROLL_LEFT:
      scroll_left_ = value->data_.i;
      break;
    default:
      break;
  }
  if (!IsInvisible()) {
    RenderCommand* cmd = lynx_new RendererDataUpdateCommand(
        impl(), key, value, RenderCommand::CMD_SET_DATA);
    render_tree_host_->UpdateRenderObject(cmd);
  }
}

void RenderObject::HandleFixedStyle() {
  CSSStyleType cur_fixed_state = style_.css_position_type();
  if (parent_ == NULL)
    return;

  if (is_fixed_ &&
      (cur_fixed_state != CSSStyleType::CSS_POSITION_FIXED ||
       style_.visible_ == CSSStyleType::CSS_HIDDEN ||
       style_.css_display_type_ == CSSStyleType::CSS_DISPLAY_NONE)) {
    static_cast<RenderObject*>(parent_)->RemoveFixedChild(this);
  } else if (!is_fixed_ &&
             cur_fixed_state == CSSStyleType::CSS_POSITION_FIXED &&
             style_.visible_ == CSSStyleType::CSS_VISIBLE &&
             style_.css_display_type_ != CSSStyleType::CSS_DISPLAY_NONE) {
    static_cast<RenderObject*>(parent_)->AddFixedChild(this);
  }
}

void RenderObject::AddFixedChildIfHave(RenderObject* child) {
  if (child->style_.css_position_type() == CSSStyleType::CSS_POSITION_FIXED) {
    child->HandleFixedStyle();
  }
  if (child->fixed_children_.size() > 0) {
    for (int i = 0; i < child->fixed_children_.size(); ++i) {
      RenderObject* fixed = child->fixed_children_.at(i);
      fixed->HandleFixedStyle();
    }
  }
}

void RenderObject::RemoveFixedChildIfHave(RenderObject* removed) {
  if (removed->style_.css_position_type() == CSSStyleType::CSS_POSITION_FIXED) {
    removed->HandleFixedStyle();
  }

  while (removed->fixed_children_.size() > 0) {
    RenderObject* fixed = removed->fixed_children_.at(0);
    fixed->HandleFixedStyle();
  }
}

void RenderObject::AddFixedChild(RenderObject* fixed_child) {
  fixed_children_.push_back(fixed_child);
  if (this == fixed_child->parent_) {
    // Remove from parent
    RenderCommand* cmd_move_from_parent = lynx_new RendererOperatorCommand(
        impl(), fixed_child->impl_.Get(), 0, RenderCommand::CMD_REMOVE_VIEW);
    render_tree_host_->UpdateRenderObject(cmd_move_from_parent);

    // Move to body
    RenderObject* root = render_tree_host_->render_root();
    RenderCommand* cmd_move_to_body =
        lynx_new RendererOperatorCommand(root->impl(), fixed_child->impl_.Get(),
                                         -1, RenderCommand::CMD_ADD_VIEW);
    render_tree_host_->UpdateRenderObject(cmd_move_to_body);
    fixed_child->is_fixed_ = true;
  }
  if (parent_ != NULL) {
    static_cast<RenderObject*>(parent_)->AddFixedChild(fixed_child);
  }
}

void RenderObject::RemoveFixedChild(RenderObject* fixed_child) {
  for (auto it = fixed_children_.cbegin(); it != fixed_children_.cend(); ++it) {
    if (*it == fixed_child) {
      fixed_children_.erase(it);
      if (parent_ != NULL) {
        static_cast<RenderObject*>(parent_)->RemoveFixedChild(fixed_child);
      }
      break;
    }
  }
  if (this == fixed_child->parent_) {
    // Remove from body
    RenderObject* root = render_tree_host_->render_root();
    RenderCommand* cmd_remove_from_body =
        lynx_new RendererOperatorCommand(root->impl(), fixed_child->impl_.Get(),
                                         0, RenderCommand::CMD_REMOVE_VIEW);
    render_tree_host_->UpdateRenderObject(cmd_remove_from_body);
    fixed_child->is_fixed_ = false;

    // Move to body
    int index = Find(fixed_child);
    RenderCommand* cmd_move_to_parent = lynx_new RendererOperatorCommand(
        impl(), fixed_child->impl_.Get(), index, RenderCommand::CMD_ADD_VIEW);
    render_tree_host_->UpdateRenderObject(cmd_move_to_parent);
  }
}

void RenderObject::ReceiveCanvasRenderCmd(base::ScopedPtr<base::CanvasRenderCommand>& cmd_single){
    if(canvas_cmds_.Get() == NULL){
        canvas_cmds_.Reset(lynx_new jscore::LynxArray());
    }
    if(cmd_single->cmd_type_.compare("drawCmd") == 0){
        RenderCommand* cmd = lynx_new RendererDataUpdateCommand(impl(), 5, std::move(canvas_cmds_), RenderCommand::CMD_SET_DATA);
        render_tree_host_->UpdateRenderObject(cmd);
    }else if(cmd_single->cmd_type_.compare("appendCmd") == 0){
        RenderCommand* cmd = lynx_new RendererDataUpdateCommand(impl(), 6, std::move(canvas_cmds_), RenderCommand::CMD_SET_DATA);
        render_tree_host_->UpdateRenderObject(cmd);
    }else{
        canvas_cmds_->Push(cmd_single.Release());
    }
};

}  // namespace lynx
