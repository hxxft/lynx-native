// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_STRING_UTILS_H_
#define LYNX_BASE_STRING_UTILS_H_

#include <string>
#include <vector>

namespace base {
    bool SplitString(const std::string& target, char separator, std::vector<std::string>& result);
}

#endif //LYNX_BASE_STRING_UTILS_H_
