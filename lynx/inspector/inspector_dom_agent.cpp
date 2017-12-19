#include "inspector/inspector_dom_agent.h"
#include "inspector/inspector.h"

#include "render/render_object.h"

#include "runtime/runtime.h"
#include "render/render_tree_host.h"
#include "render/label.h"


namespace {
    Json::Value makeNode(const std::string& node_name, int id) {
        Json::Value node;
        node["nodeId"] = id;
        node["backendNodeId"] = id;
        node["nodeType"] = node_name == "#document" ? 9 : 1;
        node["nodeName"] = node_name;
        node["localName"] = node_name;
        node["nodeValue"] = Json::Value(Json::stringValue);
        node["childNodeCount"] = 0;
        node["attributes"] = Json::Value(Json::arrayValue);
        node["children"] = Json::Value(Json::arrayValue);;
        return node;
    }
    
    Json::Value toNode(lynx::RenderObject* renderer) {
        Json::Value node;
        node["nodeId"] = renderer->id();
        node["backendNodeId"] = renderer->id();
        node["nodeType"] = renderer->render_object_type() == lynx::LYNX_TEXT_NODE ? 3 : 1;
        node["nodeName"] = renderer->tag_name() == "body" ? "body" : "DIV";
        node["localName"] = "div";//renderer->tag_name();
        node["nodeValue"] = renderer->render_object_type() == lynx::LYNX_TEXT_NODE ? renderer->GetText() : Json::Value(Json::stringValue);
        node["childNodeCount"] = renderer->GetChildCount();
        if(renderer->Parent()) {
            node["parentId"] = const_cast<lynx::RenderObject*>(renderer->Parent())->id();
        }
        
        Json::Value attriubtes(Json::arrayValue);
        int attr_count = 0;
        attriubtes[attr_count++] = "tag";
        attriubtes[attr_count++] = renderer->tag_name();
        
        if(!renderer->attributes().empty()) {
            for(auto iter = renderer->attributes().begin();
                iter != renderer->attributes().end(); ++iter) {
                attriubtes[attr_count++] = iter->first;
                attriubtes[attr_count++] = iter->second;
            }
        }
        
        
        if(!renderer->styles().empty()) {
            std::string styles;
            for(auto iter = renderer->styles().begin();
                iter != renderer->styles().end(); ++iter) {
                styles += iter->first;
                styles += ": ";
                styles += iter->second;
                styles += ";";
            }
            attriubtes[attr_count++] = "style";
            attriubtes[attr_count++] = styles;
        }

        
        node["attributes"] = attriubtes;
//        if(renderer->render_object_type() == lynx::LYNX_BODY) {
//            return node;
//        }
        int child_count = renderer->GetChildCount();
        Json::Value children(Json::arrayValue);
        if(renderer->GetChildCount() > 0) {

            for(int i = 0; i < child_count; ++i) {
                children[i] = toNode(const_cast<lynx::RenderObject*>(renderer->Get(i)));;
            }
        }else if(renderer->render_object_type() == lynx::LYNX_LABEL){
            children[0] = toNode(static_cast<lynx::Label*>(renderer)->text_node());;
        }
        node["children"] = children;
        return node;
    }
    
    lynx::RenderObject* findRenderer(lynx::RenderObject* root, uint64_t id) {
        if(root == nullptr) return nullptr;
        if(root->id() == id) return root;
        
        int child_count = root->GetChildCount();
        if(root->GetChildCount() > 0) {
            for(int i = 0; i < child_count; ++i) {
                lynx::RenderObject* renderer = findRenderer(const_cast<lynx::RenderObject*>(root->Get(i)), id);
                if(renderer) return renderer;
            }
        }
        return nullptr;
    }
}

namespace debug {
    
    InspectorDOMAgent::InspectorDOMAgent(jscore::Runtime* runtime, Inspector* inspector):runtime_(runtime), inspector_(inspector) {
        functions_map_["DOM.getDocument"] = &InspectorDOMAgent::GetDocument;
        functions_map_["DOM.requestChildNodes"] = &InspectorDOMAgent::RequestChildNodes;
        functions_map_["DOM.getBoxModel"] = &InspectorDOMAgent::GetBoxModel;
    }
    
    InspectorDOMAgent::~InspectorDOMAgent(){
        
    }
    
    Json::Value InspectorDOMAgent::GetDocument(const Json::Value& params) {
#ifndef INSPECTOR_TEST
        lynx::RenderObject* renderer = runtime_->render_tree_host()->render_root();
        Json::Value content;
//        Json::Value root = makeNode("#document", 1);
//        Json::Value body = toNode(renderer);
//        Json::Value head = makeNode("head", 3);
//        Json::Value document = makeNode("html", 2);
//        document["childNodeCount"] = 2;
//        document["children"][0] = head;
//        document["children"][1] = body;
//        root["childNodeCount"] = 1;
//        root["children"][0] = document;
        content["root"] = toNode(renderer);
        return content;
#else

        std::string doc = "{\"id\":25,\"result\":{\"root\":{\"nodeId\":1,\"backendNodeId\":1,\"nodeType\":9,\"nodeName\":\"#document\",\"localName\":\"\",\"nodeValue\":\"\",\"childNodeCount\":1,\"children\":[{\"nodeId\":2,\"parentId\":1,\"backendNodeId\":2,\"nodeType\":1,\"nodeName\":\"HTML\",\"localName\":\"html\",\"nodeValue\":\"\",\"childNodeCount\":2,\"children\":[{\"nodeId\":3,\"parentId\":2,\"backendNodeId\":3,\"nodeType\":1,\"nodeName\":\"HEAD\",\"localName\":\"head\",\"nodeValue\":\"\",\"childNodeCount\":0,\"attributes\":[]},{\"nodeId\":4,\"parentId\":2,\"backendNodeId\":4,\"nodeType\":1,\"nodeName\":\"BODY\",\"localName\":\"body\",\"nodeValue\":\"\",\"childNodeCount\":1,\"attributes\":[]}],\"attributes\":[],\"frameId\":\"(727803537C642C28AC49AA5681CD3604)\"}],\"documentURL\":\"file:///Users/chen/Downloads/index%E7%9A%84%E5%89%AF%E6%9C%AC.html\",\"baseURL\":\"file:///Users/chen/Downloads/index%E7%9A%84%E5%89%AF%E6%9C%AC.html\",\"xmlVersion\":\"\"}}}";
        Json::Value content;
        Json::Reader reader;
        reader.parse(doc, content, false);
        return content["result"];
#endif
    }
    
    Json::Value InspectorDOMAgent::RequestChildNodes(const Json::Value& params) {
#ifndef INSPECTOR_TEST
        lynx::RenderObject* renderer = findRenderer(runtime_->render_tree_host()->render_root(), params["nodeId"].asInt64());

        int child_count = renderer->GetChildCount();
        Json::Value children(Json::arrayValue);
        if(child_count > 0) {
            for(int i = 0; i < child_count; ++i) {
                children[i] = toNode(const_cast<lynx::RenderObject*>(renderer->Get(i)));;
            }
        }else if(renderer->render_object_type() == lynx::LYNX_LABEL) {
            children[0] = toNode(static_cast<lynx::Label*>(renderer)->text_node());
        }

        Json::Value content;
        Json::Value content_params;
        content["method"] = "DOM.setChildNodes";
        content_params["parentId"] = renderer->id();
        content_params["nodes"] = children;
        content["params"] = content_params;

        inspector_->Send(content);
#else
        
        
        std::string child = "{\"method\":\"DOM.setChildNodes\",\"params\":{\"parentId\":4,\"nodes\":[{\"nodeId\":5,\"parentId\":4,\"backendNodeId\":5,\"nodeType\":1,\"nodeName\":\"DIV\",\"localName\":\"div\",\"nodeValue\":\"\",\"childNodeCount\":1,\"attributes\":[\"tag\",\"view\",\"class\",\"view-container\",\"style\",\"width: 700;margin-top: 20;margin-bottom: 20;margin-left: 10;margin-right: 10;padding-top: 20;padding-bottom: 20;border-width: 2;border-color: #ccc;border-radius: 8;flex-direction: column;justify-content: center;align-items: center;\"]}]}}";
        Json::Value content;
        Json::Reader reader;
        reader.parse(child, content, false);
        inspector_->Send(content);
#endif
        return Json::Value(Json::objectValue);
    }
    
    Json::Value InspectorDOMAgent::GetBoxModel(const Json::Value& params) {
        uint64_t id = params["nodeId"].asInt64();
        lynx::RenderObject* renderer = findRenderer(runtime_->render_tree_host()->render_root(), id);
        if(renderer == nullptr) return Json::Value(Json::objectValue);
        Json::Value box_model;
        
        Json::Value content;
        content[0] = renderer->measured_position_.left_;
        content[1] = renderer->measured_position_.top_;
        content[2] = renderer->measured_position_.right_;
        content[3] = renderer->measured_position_.top_;
        content[4] = renderer->measured_position_.right_;
        content[5] = renderer->measured_position_.bottom_;
        content[6] = renderer->measured_position_.left_;
        content[7] = renderer->measured_position_.bottom_;
        box_model["content"] = content;
        
        Json::Value border;
        border[0] = renderer->measured_position_.left_;
        border[1] = renderer->measured_position_.top_;
        border[2] = renderer->measured_position_.right_;
        border[3] = renderer->measured_position_.top_;
        border[4] = renderer->measured_position_.right_;
        border[5] = renderer->measured_position_.bottom_;
        border[6] = renderer->measured_position_.left_;
        border[7] = renderer->measured_position_.bottom_;
        box_model["border"] = border;
        
        Json::Value padding;
        padding[0] = renderer->measured_position_.left_;
        padding[1] = renderer->measured_position_.top_;
        padding[2] = renderer->measured_position_.right_;
        padding[3] = renderer->measured_position_.top_;
        padding[4] = renderer->measured_position_.right_;
        padding[5] = renderer->measured_position_.bottom_;
        padding[6] = renderer->measured_position_.left_;
        padding[7] = renderer->measured_position_.bottom_;
        box_model["padding"] = padding;
        
        Json::Value margin;
        margin[0] = renderer->measured_position_.left_;
        margin[1] = renderer->measured_position_.top_;
        margin[2] = renderer->measured_position_.right_;
        margin[3] = renderer->measured_position_.top_;
        margin[4] = renderer->measured_position_.right_;
        margin[5] = renderer->measured_position_.bottom_;
        margin[6] = renderer->measured_position_.left_;
        margin[7] = renderer->measured_position_.bottom_;
        box_model["margin"] = margin;
        
        box_model["width"] = renderer->measured_size_.width_;
        box_model["height"] = renderer->measured_size_.height_;
        Json::Value model;
        model["model"] = box_model;
        return model;
    }
    
    Json::Value InspectorDOMAgent::CallMethod(Json::Value& content) {
        std::string method = content["method"].asString();
        Json::Value params = content["params"];
        
        auto iter = functions_map_.find(method);
        if(iter == functions_map_.end()) {
            Json::Value result;
            result["result"] = Json::Value(Json::objectValue);
            result["id"] = content["id"].asInt();
            return result;
        }
        
        Json::Value result;
        Json::Value root(Json::objectValue);
    
        result["result"] = (this->*(iter->second))(params);
        result["id"] = content["id"].asInt();
        
        return result;
    }
}
