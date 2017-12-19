#ifndef LYNX_INSPECTOR_INSPECTOR_CSS_AGENT_H_
#define LYNX_INSPECTOR_INSPECTOR_CSS_AGENT_H_

#include "inspector/inspector_agent_base.h"

#include <unordered_map>

#include "render/render_object.h"

namespace jscore {
    class Runtime;
}

namespace debug {
    class InspectorCSSAgent : public InspectorAgentBase {
    public:
        InspectorCSSAgent(jscore::Runtime* runtime);
        virtual ~InspectorCSSAgent();
        virtual Json::Value CallMethod(Json::Value& content);
    private:
        typedef Json::Value (InspectorCSSAgent::*CSSAgentMethod)(const Json::Value& params);
        
        Json::Value GetMatchedStylesForNode(const Json::Value& params);
        Json::Value GetComputedStyleForNode(const Json::Value& params);
        Json::Value GetInlineStylesForNode(const Json::Value& params);
        
        jscore::Runtime* runtime_;
        std::map<std::string, CSSAgentMethod> functions_map_;
    };
}

#endif
