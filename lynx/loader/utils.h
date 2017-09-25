// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_LOADER_UTILS_H_
#define LYNX_LOADER_UTILS_H_

#include "net/url_parser.h"

namespace loader {

    static std::string ToCompleteUrl(const std::string& url, const std::string& ref_url) {
        if (ref_url.empty()) {
            return url;
        }

        net::URLParser parser(ref_url);
        std::string converted_url = parser.URLWithoutParams();

        if (strncmp(url.c_str(), "http", 4) == 0) {
            converted_url = url;
        } else if (strncmp(url.c_str(), "./", 2) == 0) {
            converted_url = url.substr(1, url.size());
            converted_url = parser.BaseUrl() + converted_url;
        } else if (strncmp(url.c_str(), "/", 1) == 0) {
            converted_url = parser.BaseUrl() + url;
        } else {
            converted_url = parser.URLWithoutParams() + "/" + url;
        }

        return converted_url;
    }

}

#endif // LYNX_LOADER_UTILS_H_
