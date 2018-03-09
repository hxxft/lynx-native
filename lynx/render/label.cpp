// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/label.h"
#include "render/label_measurer.h"
#include "render/impl/render_object_impl.h"
#include "render/impl/render_command.h"
#include "render/render_tree_host.h"

namespace lynx {
TextNode::TextNode(jscore::ThreadManager* manager,
                   const char* tag_name,
                   uint64_t id,
                   RenderTreeHost* host)
        : RenderObject(tag_name,
                       LYNX_TEXT_NODE,
                       id,
                       NULL,
                       host),
            label_(NULL){
}
TextNode::~TextNode() {
}

void TextNode::SetText(const std::string &text) {
    text_ = text;
    if(label_) {
        RenderCommand* cmd = lynx_new RendererAttrUpdateCommand(
                label_->impl(), "", text, RenderCommand::CMD_SET_LABEL_TEXT);
        render_tree_host_->UpdateRenderObject(cmd);
        label_->Dirty();
    }
}

Label::Label(jscore::ThreadManager* manager,
             const char* tag_name,
             uint64_t id,
             RenderTreeHost* host)
    : RenderObject(tag_name,
                   LYNX_LABEL,
                   id,
                   RenderObjectImpl::Create(manager, LYNX_LABEL),
                   host),
      text_node_(NULL){
}

base::Size Label::OnMeasure(int width_descriptor, int height_descriptor) {
    if(text_node_ == NULL) {
        return base::Size();
    }
    int width_wanted = (int) style_.width_;
    int height_wanted = (int) style_.height_;
    int width_mode = base::Size::Descriptor::GetMode(width_descriptor);
    int height_mode = base::Size::Descriptor::GetMode(height_descriptor);
    width_descriptor = base::Size::Descriptor::GetSize(width_descriptor);
    height_descriptor = base::Size::Descriptor::GetSize(height_descriptor);

    width_descriptor = CSS_IS_UNDEFINED(width_wanted)
            && !CSS_IS_UNDEFINED(width_descriptor)
            && (width_mode == base::Size::Descriptor::EXACTLY
                || width_mode == base::Size::Descriptor::AT_MOST)?
            (int) style_.ClampExactWidth(width_descriptor) :
            (int) style_.ClampWidth();
    height_descriptor = CSS_IS_UNDEFINED(height_wanted)
             && !CSS_IS_UNDEFINED(height_descriptor)
             && (height_mode == base::Size::Descriptor::EXACTLY
                 || height_mode == base::Size::Descriptor::AT_MOST)?
             (int) style_.ClampExactHeight(height_descriptor) :
             (int) style_.ClampHeight();

    width_descriptor -= style_.padding_left_ + style_.padding_right_
             + style_.border_width_ * 2;
    height_descriptor -= style_.padding_top_ + style_.padding_bottom_
              + style_.border_width_ * 2;

    base::Size size =
            LabelMeasurer::MeasureLabelSizeAndSetTextLayout(
                    this,
                    base::Size(width_descriptor, height_descriptor),
                    text_node_ == NULL ? GetText() : text_node_->GetText());


    size.width_ += style_.padding_left_ + style_.padding_right_
                   + style_.border_width_ * 2;
    size.height_ += style_.padding_top_ + style_.padding_bottom_
                    + style_.border_width_ * 2;

    size.width_ = (int) style_.ClampWidth(size.width_);
    size.height_ = (int) style_.ClampHeight(size.height_);


    size.width_ = !CSS_IS_UNDEFINED(width_descriptor)
                          && width_mode == base::Size::Descriptor::EXACTLY ?
                  (int) style_.ClampExactWidth(base::Size::Descriptor::GetSize(width_descriptor)) :
                  (int) style_.ClampWidth(size.width_);
    size.height_ = !CSS_IS_UNDEFINED(height_descriptor)
                           && height_mode == base::Size::Descriptor::EXACTLY ?
                   (int) style_.ClampExactHeight(base::Size::Descriptor::GetSize(height_descriptor)) :
                   (int) style_.ClampHeight(size.height_);

    measured_size_ = size;
    return size;
}

void Label::InsertChild(ContainerNode* child, int index) {
    text_node_ = static_cast<TextNode*>(child);
    text_node_->label_ = this;
    if(!text_node_->GetText().empty()) {
        RenderCommand* cmd = lynx_new RendererAttrUpdateCommand(
                impl(), "", text_node_->GetText(), RenderCommand::CMD_SET_LABEL_TEXT);
        render_tree_host_->UpdateRenderObject(cmd);
        Dirty();
    }
}

void Label::InsertBefore(RenderObject* child, RenderObject* reference) {
    InsertChild(child, -1);
}

}  // namespace lynx
