// Copyright 2017 The Lynx Authors. All rights reserved.

#include <string>
#include <vector>
#include "runtime/base/lynx_map.h"
#include "runtime/base/lynx_value.h"

#ifndef LYNX_CANVAS_CMD_H
#define LYNX_CANVAS_CMD_H

namespace base{
    class CanvasRenderCommand : public jscore::LynxMap {
    public:
        CanvasRenderCommand():
                cmd_type_(""){}

        virtual ~CanvasRenderCommand(){}

        CanvasRenderCommand(std::string type) :
                cmd_type_(type){
            jscore::LynxMap::Set("type", jscore::LynxValue::MakeString(type).Release());
        }
        std::string cmd_type_;
    };
}
#endif //LYNX_CANVAS_CMD_H