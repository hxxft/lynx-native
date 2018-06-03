// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/lazy_instance.h"
#include "base/log/logging.h"
#include "base/ref_counted_ptr.h"
#include "content/lynx_thread.h"
#include "plugin/base/plugin.h"
#include "plugin/plugin_client.h"
#include "plugin/plugin_server.h"

#include "base/threading/completion_event.h"

#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace plugin {
namespace {

class PluginServerImpl : public base::RefCountPtr<PluginServerImpl> {
 public:
  PluginServerImpl() {}

  void Register(PluginClient* client, base::CompletionEvent* completion) {
    clients_.insert(client);
    completion->Signal();
  }

  void UnRegister(PluginClient* client, base::CompletionEvent* completion) {
    for (EventMap::iterator iter = event_map_.begin(); iter != event_map_.end();
         ++iter) {
      if (iter->second.find(client) != iter->second.end()) {
        RemoveEvent(client, iter->first.first, iter->first.second);
      }
    }
    Clients::iterator iter = clients_.find(client);
    if (iter != clients_.end()) {
      clients_.erase(iter);
      completion->Signal();
    }
  }

  void Exec(PluginClient* client, base::ScopedPtr<jscore::LynxArray> array) {
    std::string module(array->Get(0)->data_.str);
    Plugin* plugin = FindPlugin(module);
    if (plugin) {
      plugin->Exec(reinterpret_cast<long>(client), array);
    }
    return;
  }

  void Return(long client_id,
              int method_id,
              bool successed,
              base::ScopedPtr<jscore::LynxArray> array) {
    PluginClient* plugin_client = reinterpret_cast<PluginClient*>(client_id);
    Clients::iterator iter = clients_.find(plugin_client);
    if (iter != clients_.end()) {
      plugin_client->RunOnClientThread(
          base::Bind(&PluginServerImpl::ReturnOnClientThread,
                     base::ScopedRefPtr<PluginServerImpl>(this), plugin_client,
                     method_id, successed, array));
    }
    return;
  }

  void AddEvent(PluginClient* client,
                const std::string& name,
                const std::string& event) {
    std::pair<std::string, std::string> plugin_event(name, event);
    EventMap::iterator plugin_event_iter = event_map_.find(plugin_event);
    if (plugin_event_iter == event_map_.end()) {
      Plugin* plugin = FindPlugin(name);
      if (plugin) {
        plugin->AddEvent(event);
      }

      std::unordered_set<PluginClient*> client_set;
      client_set.insert(client);
      event_map_.insert(make_pair(plugin_event, client_set));
    } else {
      plugin_event_iter->second.insert(client);
    }
  }

  void RemoveEvent(PluginClient* client,
                   const std::string& name,
                   const std::string& event) {
    std::pair<std::string, std::string> plugin_event(name, event);
    EventMap::iterator plugin_event_iter = event_map_.find(plugin_event);
    if (plugin_event_iter != event_map_.end()) {
      plugin_event_iter->second.erase(client);
      if (plugin_event_iter->second.empty()) {
        Plugins::iterator plugin_iter = plugins_.find(name);
        if (plugin_iter != plugins_.end()) {
          plugin_iter->second->RemoveEvent(event);
        }
      }
    }
  }

  void DispatchEvent(const std::string& name,
                     const std::string& event,
                     base::ScopedPtr<jscore::LynxArray> array) {
    std::pair<std::string, std::string> plugin_event(name, event);
    EventMap::iterator plugin_event_iter = event_map_.find(plugin_event);
    if (plugin_event_iter != event_map_.end()) {
      for (std::unordered_set<PluginClient*>::iterator client_iter =
               plugin_event_iter->second.begin();
           client_iter != plugin_event_iter->second.end(); ++client_iter) {
        base::ScopedPtr<jscore::LynxArray> result(array->Clone());
        std::string plugin_event = name + "#" + event;
        (*client_iter)
            ->RunOnClientThread(
                base::Bind(&PluginServerImpl::DispatchEventOnClientThread,
                           base::ScopedRefPtr<PluginServerImpl>(this),
                           (*client_iter), plugin_event, result));
      }
    }
  }

 private:
  Plugin* FindPlugin(const std::string& name) {
    Plugin* plugin = nullptr;
    Plugins::iterator iter = plugins_.find(name);
    if (iter != plugins_.end()) {
      plugin = iter->second;
    } else {
      plugin = RegisterPlugin(name);
    }
    if (!plugin) {
      DLOG(ERROR) << "Cant't find plugin " << name;
    }
    return plugin;
  }

  Plugin* RegisterPlugin(const std::string& name) {
    Plugin* plugin = Plugin::Create(name);
    plugins_.add(name, plugin);
    return plugin;
  }

  void ReturnOnClientThread(PluginClient* client,
                            int method,
                            bool successed,
                            base::ScopedPtr<jscore::LynxArray> result) {
    client->Return(method, successed, result);
  }

  void DispatchEventOnClientThread(PluginClient* client,
                                   const std::string& event,
                                   base::ScopedPtr<jscore::LynxArray> result) {
    client->DispatchEvent(event, result);
  }

  typedef std::map<std::pair<std::string, std::string>,
                   std::unordered_set<PluginClient*>>
      EventMap;
  EventMap event_map_;

  typedef std::unordered_set<PluginClient*> Clients;
  Clients clients_;

  typedef base::ScopedPtrMap<std::string, Plugin> Plugins;
  Plugins plugins_;
};

base::LazyInstance<PluginServerImpl> g_plugin_server;
}  // namespace

void PluginServer::Register(PluginClient* client) {
  base::CompletionEvent* completion = lynx_new base::CompletionEvent;
  content::LynxThread::PostTask(
      content::LynxThread::PLUGIN,
      base::Bind(&PluginServerImpl::Register,
                 base::ScopedRefPtr<PluginServerImpl>(g_plugin_server.Get()),
                 client, completion));
  completion->Wait();
  lynx_delete(completion);
}

void PluginServer::UnRegister(PluginClient* client) {
  base::CompletionEvent* completion = lynx_new base::CompletionEvent;
  content::LynxThread::PostTask(
      content::LynxThread::PLUGIN,
      base::Bind(&PluginServerImpl::UnRegister,
                 base::ScopedRefPtr<PluginServerImpl>(g_plugin_server.Get()),
                 client, completion));
  completion->Wait();
  lynx_delete(completion);
}

void PluginServer::Exec(PluginClient* client,
                        base::ScopedPtr<jscore::LynxArray>& array) {
  content::LynxThread::PostTask(
      content::LynxThread::PLUGIN,
      base::Bind(&PluginServerImpl::Exec,
                 base::ScopedRefPtr<PluginServerImpl>(g_plugin_server.Get()),
                 client, array));
}

void PluginServer::Return(long client_id,
                          int method_id,
                          bool successed,
                          base::ScopedPtr<jscore::LynxArray>& array) {
  content::LynxThread::PostTask(
      content::LynxThread::PLUGIN,
      base::Bind(&PluginServerImpl::Return,
                 base::ScopedRefPtr<PluginServerImpl>(g_plugin_server.Get()),
                 client_id, method_id, successed, array));
}

void PluginServer::AddEvent(PluginClient* client,
                            const std::string& name,
                            const std::string& event) {
  content::LynxThread::PostTask(
      content::LynxThread::PLUGIN,
      base::Bind(&PluginServerImpl::AddEvent,
                 base::ScopedRefPtr<PluginServerImpl>(g_plugin_server.Get()),
                 client, name, event));
}

void PluginServer::RemoveEvent(PluginClient* client,
                               const std::string& name,
                               const std::string& event) {
  content::LynxThread::PostTask(
      content::LynxThread::PLUGIN,
      base::Bind(&PluginServerImpl::RemoveEvent,
                 base::ScopedRefPtr<PluginServerImpl>(g_plugin_server.Get()),
                 client, name, event));
}

void PluginServer::DispatchEvent(const std::string& name,
                                 const std::string& event,
                                 base::ScopedPtr<jscore::LynxArray>& array) {
  content::LynxThread::PostTask(
      content::LynxThread::PLUGIN,
      base::Bind(&PluginServerImpl::DispatchEvent,
                 base::ScopedRefPtr<PluginServerImpl>(g_plugin_server.Get()),
                 name, event, array));
}

void PluginServer::Initialize() {
  g_plugin_server.Get()->AddRef();
}

}  // namespace plugin
