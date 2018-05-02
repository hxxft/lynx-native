#ifndef LYNX_INSPECTOR_INSPECTOR_H_
#define LYNX_INSPECTOR_INSPECTOR_H_

#include "base/threading/thread.h"
#include "base/scoped_ptr.h"

#include "inspector/inspector_session.h"

namespace jscore {
    class Runtime;
}

namespace debug {
    class InspectorAgentBase;
    class Inspector : public InspectorSession::Dispatcher, public base::RefCountPtr<Inspector>{
    public:
        Inspector(base::Thread* js_thread, const std::string& name);
        
        void Attach(jscore::Runtime* runtime, const std::string& address = "127.0.0.1", int port = 9526);
        void Detach() {}
        void Send(Json::Value content);
        
        virtual void DispatchMessage(const base::ScopedRefPtr<base::IOBuffer>& data_buffer, uint64_t size);
    private:
        
        void AttachToInspectorThread(const std::string& address, int port);
        void SendToInspectorThread(const std::string& command_type, Json::Value content);
        void DispatchMessageToJSThread(const std::string& message);
        
        Json::Reader reader_;
        jscore::Runtime* runtime_;
        base::Thread* js_thread_;
        base::ScopedPtr<base::Thread> inspector_thread_;
        base::ScopedPtr<InspectorSession> session_;
        base::ScopedPtrMap<std::string, InspectorAgentBase> agent_map_;
    };
}


#endif
