// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_IMPL_RENDER_COMMAND_H_
#define LYNX_RENDER_IMPL_RENDER_COMMAND_H_

#include <string>


#include "base/task/callback.h"
#include "base/position.h"
#include "base/size.h"
#include "base/weak_ptr.h"
#include "runtime/base/lynx_value.h"
#include "runtime/base/lynx_array.h"
#include "runtime/base/lynx_map.h"
#include "runtime/canvas_cmd.h"
#include "layout/css_style.h"

namespace lynx {
class RenderObjectImpl;
class RenderCommand : public base::Closure {
 public:
    explicit RenderCommand(RenderObjectImpl* host, int type);
    virtual ~RenderCommand();

    enum CommandType {
        CMD_SET_POSITION,
        CMD_SET_SIZE,
        CMD_SET_STYLE,
        CMD_SET_ATTR,
        CMD_ADD_VIEW,
        CMD_REMOVE_VIEW,
        CMD_SET_LABEL_TEXT,
        CMD_ADD_EVENT_LISTENER,
        CMD_REMOVE_EVENT_LISTENER,
        CMD_SET_DATA,
        CMD_ANIMATE,
        CMD_CANCEL_ANIMATION,
    };

    virtual void Execute() = 0;

    virtual void Run() {
        Execute();
    }

 protected:
    RenderObjectImpl* host_;
    int type_;
};

    class RendererOperatorCommand : public RenderCommand {
    public:
        explicit RendererOperatorCommand(RenderObjectImpl* host, RenderObjectImpl* child, int index, int type)
                : RenderCommand(host, type),
                  child_(child),
                  index_(index){

        }
        virtual ~RendererOperatorCommand() {}
        virtual void Execute();

    private:
        base::ScopedRefPtr<RenderObjectImpl> child_;
        int index_;
    };

    class RendererPosUpdateCommand : public RenderCommand {
    public:
        explicit RendererPosUpdateCommand(RenderObjectImpl* host, base::Position& position, int type)
        : RenderCommand(host, type),
        position_(position) {

        }
        virtual ~RendererPosUpdateCommand() {}
        virtual void Execute();
    private:
        base::Position position_;
    };

    class RendererSizeUpdateCommand : public RenderCommand {
    public:
        explicit RendererSizeUpdateCommand(RenderObjectImpl* host, base::Size& size, int type)
                : RenderCommand(host, type),
                  size_(size) {

        }
        virtual ~RendererSizeUpdateCommand() {}
        virtual void Execute();

    private:
        base::Size size_;
    };

    class RendererStyleUpdateCommand : public RenderCommand {
    public:
        explicit RendererStyleUpdateCommand(RenderObjectImpl* host, CSSStyle& style, int type)
                : RenderCommand(host, type), style_(style) {

        }
        virtual ~RendererStyleUpdateCommand() {}
        virtual void Execute();

    private:
        CSSStyle style_;
    };

    class RendererAttrUpdateCommand : public RenderCommand {
    public:
        explicit RendererAttrUpdateCommand(RenderObjectImpl* host, const std::string& key, const std::string& value, int type)
                : RenderCommand(host, type), key_(key), value_(value) {

        }
        virtual ~RendererAttrUpdateCommand() {}
        virtual void Execute();

    private:
        std::string key_;
        std::string value_;
    };

    class RendererEventUpdateCommand : public RenderCommand {
    public:
        explicit RendererEventUpdateCommand(RenderObjectImpl* host, const std::string& event, int type)
                : RenderCommand(host, type), event_(event) {

        }
        virtual void Execute();
        virtual ~RendererEventUpdateCommand() {}
    private:
        std::string event_;
    };

    class RendererDataUpdateCommand : public RenderCommand {
    public:
        explicit RendererDataUpdateCommand(RenderObjectImpl* host, int key, base::ScopedPtr<jscore::LynxValue> value, int type)
                : RenderCommand(host, type), key_(key), value_(value){

        }
        virtual ~RendererDataUpdateCommand() {}
        virtual void Execute();

    private:
        int key_;
        base::ScopedPtr<jscore::LynxValue> value_;
    };

    class RendererAnimateCommand : public RenderCommand {
    public:
        explicit RendererAnimateCommand(RenderObjectImpl* host,
                                        base::ScopedPtr<jscore::LynxArray> keyframes,
                                        base::ScopedPtr<jscore::LynxMap> options)
                : RenderCommand(host, CMD_ANIMATE), options_(options), keyframes_(keyframes){

        }
        virtual ~RendererAnimateCommand() {}
        virtual void Execute();

    private:
        base::ScopedPtr<jscore::LynxMap> options_;
        base::ScopedPtr<jscore::LynxArray> keyframes_;
    };

    class RendererCancelAnimationCommand : public RenderCommand {
    public:
        explicit RendererCancelAnimationCommand(RenderObjectImpl* host, const std::string& id)
                : RenderCommand(host, CMD_CANCEL_ANIMATION) {

        }
        virtual ~RendererCancelAnimationCommand() {}
        virtual void Execute();

    private:
        std::string id_;
    };


}  // namespace lynx

#endif  // LYNX_RENDER_IMPL_RENDER_COMMAND_H_
