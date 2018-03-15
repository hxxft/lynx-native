//
// Created by chicheng on 2018/2/28.
//

#include <string>
#include <vector>
#include "runtime/base/lynx_object.h"
#include "runtime/base/lynx_value.h"

#ifndef LYNX_CANVAS_CMD_H
#define LYNX_CANVAS_CMD_H


namespace base{
    class CanvasRenderCommand : public jscore::LynxObject {
    public:
        CanvasRenderCommand():
                cmd_type_(""){}

        virtual ~CanvasRenderCommand(){}

        CanvasRenderCommand(std::string type) :
                cmd_type_(type){
            jscore::LynxObject::Set("type", jscore::LynxValue::MakeString(type).Release());
        }
        std::string cmd_type_;
    };
}
#endif //LYNX_CANVAS_CMD_H