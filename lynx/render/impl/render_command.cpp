// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/impl/render_command.h"

#include "render/render_object.h"
#include "render/impl/render_object_impl.h"

namespace lynx {
RenderCommand::RenderCommand(RenderObjectImpl *host, int type)
    : host_(host),
      type_(type),
      weak_ptr_(this) {
    host_->AddRef();
}

RenderCommand::~RenderCommand() {
    host_->Release();
    weak_ptr_.Invalidate();
}

void RendererOperatorCommand::Execute() {
    switch (type_) {
        case CMD_ADD_VIEW:
            host_->InsertChild(child_.Get(), index_);
            break;
        case CMD_REMOVE_VIEW:
            host_->RemoveChild(child_.Get());
            break;
        default:
            break;
    }
}

    void RendererPosUpdateCommand::Execute() {
        switch (type_) {
            case CMD_SET_POSITION:
                host_->SetPosition(position_);
                host_->RequestLayout();
                break;
            default:
                break;
        }
    }

    void RendererSizeUpdateCommand::Execute() {
        switch (type_) {
            case CMD_SET_SIZE:
                host_->SetSize(size_);
                break;
            default:
                break;
        }
    }

    void RendererStyleUpdateCommand::Execute() {
        switch (type_) {
            case CMD_SET_STYLE:
                host_->UpdateStyle(style_);
                break;
            default:
                break;
        }
    }

    void RendererAttrUpdateCommand::Execute() {
        switch (type_) {
            case CMD_SET_ATTR:
                host_->SetAttribute(key_, value_);
                break;
            case CMD_SET_LABEL_TEXT:
                host_->SetText(value_);
                break;
            default:
                break;
        }
    }

    void RendererEventUpdateCommand::Execute() {
        switch (type_) {
            case CMD_ADD_EVENT_LISTENER:
                host_->AddEventListener(event_);
                break;
            case CMD_REMOVE_EVENT_LISTENER:
                host_->RemoveEventListener(event_);
                break;
            default:
                break;
        }
    }

    void RendererDataUpdateCommand::Execute() {
        switch (type_) {
            case CMD_SET_DATA:
                host_->SetData(key_, value_);
                break;
            default:
                break;
        }
    }

    void RendererAnimateCommand::Execute() {
        host_->Animate(keyframes_, options_);
    }

    void RendererCancelAnimationCommand::Execute() {
        host_->CancelAnimation(id_);
    }
}  // namespace lynx
