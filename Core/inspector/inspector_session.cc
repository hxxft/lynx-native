#include "inspector/inspector_session.h"

namespace debug {
    
    InspectorSession::InspectorSession(Dispatcher* dispatcher)
        : socket_(),
        dispatcher_(dispatcher),
        inspector_thread_(nullptr),
        js_thread_(nullptr),
        basic_info_(){
//            basic_info_.SetDeviceType("android");
//            basic_info_.SetFromType("device");
//            basic_info_.SetUUID("00000000-68e7-2645-a041-539300000000");
//            basic_info_.SetAppID("com.mogujie.xcdebugsample");
//            basic_info_.SetPageID("00000000-68e7-2645-a041-539300000000_com.mogujie.xcdebugsample_26867_1");
//            basic_info_.SetPageTitle("android-xcore-page");
//            basic_info_.SetDeviceName("Nexus 5");
    }
    
    InspectorSession::~InspectorSession() {
        
    }
    
    void InspectorSession::Start(const std::string& address, int port) {
        if(socket_.Get() == nullptr) {
            socket_.Reset(new net::WebSocket(new InspectorSocketListener(this), address, port));
        }
        socket_->Connect();
    }
    
    void InspectorSession::Send(const std::string& command_type, const std::string& content) {
        InspectorResponse response(basic_info_);
        response.SetCommandType(command_type);
        if(!content.empty()) response.SetContent(content);
        Send(response.toString());
    }
    
    void InspectorSession::Send(const std::string& command_type, Json::Value& content) {

        Send(content.toStyledString());

//        InspectorResponse response(basic_info_);
//        response.SetCommandType(command_type);
//        printf("Send content: %s", content.toStyledString().c_str());
//        response.SetContent(content.toStyledString());
//        Send(response.toString());
    }
    
    void InspectorSession::Send(const std::string& message) {
        printf("send to server: %s", message.c_str());
        socket_->SendText(message);
    }
    
    void InspectorSession::Dispatch(const base::ScopedRefPtr<base::IOBuffer>& data_buffer, uint64_t size) {
        dispatcher_->DispatchMessage(data_buffer, size);
    }
    
    
    void InspectorSocketListener::OnOpen(const base::ScopedRefPtr<base::IOBuffer>& data_buffer, uint64_t size) {
        if(delegate_) {
            delegate_->Send("register", "aaa");
            //delegate_->Send("{\"fromType\":\"device\",\"deviceType\":\"android\",\"commandType\":\"devicesAndPages\",\"resultStatus\":\"success\",\"devicesList\":[{\"udid\":\"00000000-68e7-2645-a041-539300000000\",\"deviceName\":\"Nexus 5\",\"apps\":[{\"appId\":\"com.mogujie.xcdebugsample\",\"pages\":[{\"pageId\":\"00000000-68e7-2645-a041-539300000000_com.mogujie.xcdebugsample_26867_1\",\"pageTitle\":\"android-xcore-page\",\"clientWS\":\"\",\"debugUrl\":\"http://localhost:9526/inspector/inspector.html?ws=localhost:9526/devtools/page/android/MDAwMDAwMDAtNjhlNy0yNjQ1LWEwNDEtNTM5MzAwMDAwMDAwL2NvbS5tb2d1amllLnhjZGVidWdzYW1wbGUvMDAwMDAwMDAtNjhlNy0yNjQ1LWEwNDEtNTM5MzAwMDAwMDAwX2NvbS5tb2d1amllLnhjZGVidWdzYW1wbGVfMjY4NjdfMS9hbmRyb2lkLXhjb3JlLXBhZ2U=&remoteFrontend=1\"}]}]}]}");
        }
    }
    
    void InspectorSocketListener::OnMessage(const base::ScopedRefPtr<base::IOBuffer>& data_buffer, uint64_t size) {
        if(delegate_) {
            delegate_->Dispatch(data_buffer, size);
        }
    }
    
    void InspectorSocketListener::OnClosing() {
        
    }
    
    void InspectorSocketListener::OnClosed() {
        
    }
    
    void InspectorSocketListener::OnFailure() {
        printf("OnFailure: \n");
    }
}
