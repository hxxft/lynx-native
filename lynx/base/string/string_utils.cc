// Copyright 2017 The Lynx Authors. All rights reserved.

#include "string_utils.h"
namespace base {

    bool SplitString(const std::string& target, char separator, std::vector<std::string>& result) {
        typedef std::string::size_type size_t;
        size_t i = 0;
        size_t value_start = INT_MAX;
        while (i < target.size()) {
            bool last_c = i == target.size() - 1;
            if (target[i] != separator && value_start == INT_MAX) {
                value_start = i;
            } else if (target[i] == separator || i == target.size() - 1) {
                size_t sub_num = last_c ? target.size() - value_start : i - value_start;
                if (value_start != INT_MAX) {
                    result.push_back(target.substr(value_start, sub_num));
                    value_start = INT_MAX;
                }
            }
            i++;
        }
        return !result.empty();
    }

}