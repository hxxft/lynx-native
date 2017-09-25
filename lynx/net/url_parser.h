// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_NET_URL_PARSER_H_
#define LYNX_NET_URL_PARSER_H_

#include <string>

#include "net/uri.h"

namespace net {
class URLParser {
 public:
    explicit URLParser(Uri uri) : uri_(uri) {}
    explicit URLParser(const std::string& url): uri_(Uri::Parse(url)) {}
    ~URLParser() {}

    std::string BaseUrl() {
        return uri_.protocol_ + "//" + uri_.host_;
    }

    std::string URLWithoutParams() {
        std::string url;
        url = uri_.protocol_ + "//" + uri_.host_ + uri_.path_;

        if (url.at(url.length() - 1) == '/') {
            url = url.substr(0, url.length() - 1);
        }
        return url;
    }
    
    std::string URLProtocol() {
        return uri_.protocol_;
    }

    std::string Query() {
        return uri_.query_;
    }

 private:
    Uri uri_;
};
}  // namespace net

#endif  // LYNX_NET_URL_PARSER_H_
