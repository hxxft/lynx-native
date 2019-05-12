#ifndef LYNX_INSPECTOR_INSPECTOR_RESPONSE_H_
#define LYNX_INSPECTOR_INSPECTOR_RESPONSE_H_

#include "third_party/jsoncpp/include/json/json.h"

namespace debug {

class InspectorBasicInfo {
 public:
  InspectorBasicInfo() : info_() {}

  ~InspectorBasicInfo() {}

  Json::Value& GetInfoValue() { return info_; }

  void SetDeviceType(const std::string& device_type) {
    info_["deviceType"] = device_type;
  }

  void SetFromType(const std::string& from_type) {
    info_["fromType"] = from_type;
  }

  void SetUUID(const std::string& udid) { info_["udid"] = udid; }

  void SetAppID(const std::string& app_id) { info_["appId"] = app_id; }

  void SetPageTitle(const std::string& page_title) {
    info_["pageTitle"] = page_title;
  }

  void SetPageID(const std::string& page_id) { info_["pageId"] = page_id; }

  void SetDeviceName(const std::string& device_name) {
    info_["deviceName"] = device_name;
  }

 private:
  Json::Value info_;
};

class InspectorResponse {
 public:
  explicit InspectorResponse(InspectorBasicInfo& info)
      : response_(info.GetInfoValue()) {}

  void SetCommandType(const std::string& command_type) {
    response_["commandType"] = command_type;
  }

  void SetContent(const std::string& content) {
    response_["content"] = content;
  }

  void SetContent(Json::Value& content) { response_["content"] = content; }

  std::string toString() { return response_.toStyledString(); }

 private:
  Json::Value response_;
};
}  // namespace debug

#endif
