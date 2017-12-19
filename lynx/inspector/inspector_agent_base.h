#ifndef LYNX_INSPECTOR_INSPECTOR_AGENT_BASE_H_
#define LYNX_INSPECTOR_INSPECTOR_AGENT_BASE_H_

#include <string>
#include <map>

#include "third_party/jsoncpp/include/json/json.h"

#include "base/ref_counted_ptr.h"

//#define INSPECTOR_TEST

namespace debug {
    class InspectorAgentBase : public base::RefCountPtr<InspectorAgentBase>{
    public:
        virtual Json::Value CallMethod(Json::Value& content) = 0;
    };
}

#endif
