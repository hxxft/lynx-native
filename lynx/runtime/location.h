// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_LOCATION_H_
#define LYNX_RUNTIME_LOCATION_H_

#include <string>
#include "net/uri.h"
#include "runtime/js_context.h"
#include "runtime/base/lynx_object_template.h"

namespace jscore {
    class Location : public LynxObjectTemplate {
        
    public:
        void Reload(bool force);
        void Assign(const std::string &url);
        void Replace(const std::string &url);
        void SetUrl(const std::string& url);
        void ParseHref(const std::string& href);
        
        Location(JSContext* context);
        virtual ~Location();
        
        inline std::string hash() {
            return hash_;
        }
        
        inline void set_hash(const std::string& hash) {
            hash_ = hash;
        }
        
        inline std::string host() {
            return host_;
        }
        
        inline void set_host(const std::string& host) {
            host_ = host;
        }
        
        inline std::string hostname() {
            return hostname_;
        }
        
        inline void set_hostname(const std::string& hostname) {
            hostname_ = hostname;
        }
        
        inline std::string href() {
            return href_;
        }
        
        inline void set_href(const std::string& href) {
            href_ = href;
        }
        
        inline std::string pathname() {
            return pathname_;
        }
        
        inline void set_pathname(const std::string& pathname) {
            pathname_ = pathname;
        }
        
        inline std::string port() {
            return port_;
        }
        
        inline void set_port(const std::string& port) {
            port_ = port;
        }
        
        inline std::string protocol() {
            return protocol_;
        }
        
        inline void set_protocol(const std::string& protocol) {
            protocol_ = protocol;
        }
        
        inline std::string search() {
            return search_;
        }
        
        inline void set_search(const std::string& search) {
            search_ = search;
        }
        
        inline std::string origin() {
            return origin_;
        }
        
        inline void set_origin(const std::string& origin) {
            origin_ = origin;
        }
        
    private:
        JSContext* context_;
        net::Uri uri_;
        
        std::string hash_;
        std::string host_;
        std::string hostname_;
        std::string href_;
        std::string pathname_;
        std::string port_;
        std::string protocol_;
        std::string search_;
        std::string origin_;


        static base::ScopedPtr<LynxValue>
        ReplaceCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);

        static base::ScopedPtr<LynxValue>
        ReloadCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray>& array);

        static base::ScopedPtr<LynxValue>
        AssignCallback(LynxObjectTemplate *object, base::ScopedPtr<LynxArray>& array);

        static base::ScopedPtr<LynxValue> GetHashCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetHostCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetHostNameCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetHrefCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetPathNameCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetPortCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetProtocolCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetSearchCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetOriginCallback(LynxObjectTemplate* object);

        static void SetHashCallback(LynxObjectTemplate* object,
                                    base::ScopedPtr<jscore::LynxValue> value);
        static void SetHostCallback(LynxObjectTemplate* object,
                                    base::ScopedPtr<jscore::LynxValue> value);
        static void SetHostNameCallback(LynxObjectTemplate* object,
                                        base::ScopedPtr<jscore::LynxValue> value);
        static void SetHrefCallback(LynxObjectTemplate* object,
                                    base::ScopedPtr<jscore::LynxValue> value);
        static void SetPathNameCallback(LynxObjectTemplate* object,
                                        base::ScopedPtr<jscore::LynxValue> value);
        static void SetPortCallback(LynxObjectTemplate* object,
                                    base::ScopedPtr<jscore::LynxValue> value);
        static void SetProtocolCallback(LynxObjectTemplate* object,
                                        base::ScopedPtr<jscore::LynxValue> value);
        static void SetSearchCallback(LynxObjectTemplate* object,
                                      base::ScopedPtr<jscore::LynxValue> value);
        static void SetOriginCallback(LynxObjectTemplate* object,
                                      base::ScopedPtr<jscore::LynxValue> value);
    };
}

#endif // LYNX_RUNTIME_LOCATION_H_
