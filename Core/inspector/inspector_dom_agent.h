#ifndef LYNX_INSPECTOR_INSPECTOR_DOM_AGENT_H_
#define LYNX_INSPECTOR_INSPECTOR_DOM_AGENT_H_

#include "inspector/inspector_agent_base.h"

#include <unordered_map>

#include "render/render_object.h"

namespace jscore {
class Runtime;
}

namespace debug {
class Inspector;
class InspectorDOMAgent : public InspectorAgentBase {
 public:
  InspectorDOMAgent(jscore::Runtime* runtime, Inspector* inspector);
  virtual ~InspectorDOMAgent();
  virtual Json::Value CallMethod(Json::Value& content);

 private:
  typedef Json::Value (InspectorDOMAgent::*DOMAgentMethod)(
      const Json::Value& params);

  Json::Value GetDocument(const Json::Value& params);
  Json::Value RequestChildNodes(const Json::Value& params);
  Json::Value GetBoxModel(const Json::Value& params);

  jscore::Runtime* runtime_;
  std::map<std::string, DOMAgentMethod> functions_map_;
  Inspector* inspector_;
  // std::unordered_map<uint64_t, lynx::RenderObject*> render_object_map_;
};
}  // namespace debug

#endif
