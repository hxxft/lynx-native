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
    }else{
        Dirty();
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
    int width_wanted = (int) css_style_.width_;
    int height_wanted = (int) css_style_.height_;
    int width_mode = base::Size::Descriptor::GetMode(width_descriptor);
    int height_mode = base::Size::Descriptor::GetMode(height_descriptor);
    int width = base::Size::Descriptor::GetSize(width_descriptor);
    int height = base::Size::Descriptor::GetSize(height_descriptor);

    width = CSS_IS_UNDEFINED(width_wanted)
            && !CSS_IS_UNDEFINED(width)
            && (width_mode == base::Size::Descriptor::EXACTLY
                || width_mode == base::Size::Descriptor::AT_MOST)?
            (int) css_style_.ClampExactWidth(width) :
            (int) css_style_.ClampWidth();
    height = CSS_IS_UNDEFINED(height_wanted)
             && !CSS_IS_UNDEFINED(height)
             && (height_mode == base::Size::Descriptor::EXACTLY
                 || height_mode == base::Size::Descriptor::AT_MOST)?
             (int) css_style_.ClampExactHeight(height) :
             (int) css_style_.ClampHeight();

    if (!CSS_IS_UNDEFINED(width)) {
        width -= css_style_.padding_left_ + css_style_.padding_right_
                            + css_style_.border_width_ * 2;
    }
    if (!CSS_IS_UNDEFINED(height)) {
        height -= css_style_.padding_top_ + css_style_.padding_bottom_
                  + css_style_.border_width_ * 2;
    }

    base::Size size = MeasureTextSize(base::Size(width, height));
            LabelMeasurer::MeasureLabelSizeAndSetTextLayout(
                    this, base::Size(width, height),
                    text_node_ == NULL ? GetText() : text_node_->GetText());

    size.width_ += css_style_.padding_left_ + css_style_.padding_right_
                   + css_style_.border_width_ * 2;
    size.height_ += css_style_.padding_top_ + css_style_.padding_bottom_
                    + css_style_.border_width_ * 2;

    size.width_ = !CSS_IS_UNDEFINED(width)
                          && width_mode == base::Size::Descriptor::EXACTLY ?
                  (int) css_style_.ClampExactWidth(width) :
                  (int) css_style_.ClampWidth(size.width_);
    size.height_ = !CSS_IS_UNDEFINED(height)
                           && height_mode == base::Size::Descriptor::EXACTLY ?
                   (int) css_style_.ClampExactHeight(height) :
                   (int) css_style_.ClampExactHeight(size.height_);

    measured_size_ = size;
    return size;
}

base::Size Label::MeasureTextSize(const base::Size& size) {
    return LabelMeasurer::MeasureLabelSizeAndSetTextLayout(
            this, size,
            text_node_ == NULL ? GetText() : text_node_->GetText());
}

void Label::InsertChild(ContainerNode* child, int index) {
    if(text_node_ != nullptr) {
        lynx_delete(text_node_);
    }
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
