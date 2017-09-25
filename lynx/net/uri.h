// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef uri_h
#define uri_h

#include <string>
#include <algorithm>

namespace net {
class Uri {
public:
    std::string query_, path_, protocol_, hostname_, host_, port_, href_, search_, pathname_, origin_;

    static Uri Parse(const std::string &uri) {
        Uri result;

        // href
        result.href_ = uri;

        typedef std::string::const_iterator iterator_t;

        if (uri.length() == 0)
            return result;

        iterator_t uri_end = uri.end();

        // get query start
        iterator_t query_start = std::find(uri.begin(), uri_end, '?');

        // protocol
        iterator_t protocol_start = uri.begin();
        iterator_t protocol_end = std::find(protocol_start, uri_end, ':');            //"://");

        if (protocol_end != uri_end) {
            std::string prot = &*(protocol_end);
            if ((prot.length() > 3) && (prot.substr(0, 3) == "://")) {
                result.protocol_ = std::string(protocol_start, protocol_end + 1);
                protocol_end += 3;   //      ://
            } else {
                protocol_end = uri.begin();  // no protocol
            }
        } else {
            protocol_end = uri.begin();  // no protocol
        }

        // host & hostname
        iterator_t host_start = protocol_end;
        iterator_t path_start = std::find(host_start, uri_end, '/');  // get path_start

        iterator_t host_end = (path_start != uri_end) ? path_start : query_start;
        result.host_ = std::string(host_start, host_end);

        iterator_t hostname_end = std::find(protocol_end,
                                            (path_start != uri_end) ? path_start : query_start,
                                            ':');  // check for port

        result.hostname_ = std::string(host_start, hostname_end);

        // port
        if ((hostname_end != uri_end) && ((&*(hostname_end))[0] == ':')) {
            hostname_end++;
            iterator_t port_end = (path_start != uri_end) ? path_start : query_start;
            result.port_ = std::string(hostname_end, port_end);
        }

        // path
        if (path_start != uri_end)
            result.path_ = std::string(path_start, query_start);

        // query & search
        if (query_start != uri_end) {
            result.query_ = std::string(query_start, uri.end());
            result.search_ = result.query_;
        }

        // pathname
        result.pathname_ = result.path_;

        // origin
        result.origin_ = result.protocol_ + "//" + result.host_;

        return result;

    }   // Parse
};
}
#endif
