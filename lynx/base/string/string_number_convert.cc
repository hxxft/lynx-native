// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/string/string_number_convert.h"

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>

namespace base {

bool StringToDouble(const std::string& input, double& output) {
  int old_error = errno;
  errno = 0;
  char* endptr = NULL;
  double d = strtod(input.c_str(), &endptr);
  bool valid = (errno == 0 && !input.empty() &&
                input.c_str() + input.length() == endptr && !isspace(input[0]));
  if (errno == 0)
    errno = old_error;
  if (valid)
    output = d;
  return valid;
}

bool StringToInt(const std::string& input, int64_t& output, uint8_t base = 10) {
  int old_error = errno;
  errno = 0;
  char* endptr = NULL;
  int64_t i = strtoll(input.c_str(), &endptr, base);
  bool valid = (errno == 0 && !input.empty() &&
                input.c_str() + input.length() == endptr && !isspace(input[0]));
  if (errno == 0)
    errno = old_error;
  if (valid)
    output = i;
  return valid;
}

std::vector<std::string> SplitString(const std::string& target, char separator) {
  std::vector<std::string> result;
  std::string::size_type i = 0;
  std::string::size_type value_start = INT_MAX;
  while (i < target.size()) {
    bool last_c = i == target.size() - 1;
    if (target[i] != separator && value_start == INT_MAX) {
      value_start = i;
    } else if (target[i] == separator || i == target.size() - 1) {
      std::string::size_type sub_num = last_c ? target.size() - value_start : i - value_start;
      if (value_start != INT_MAX) {
        result.push_back(target.substr(value_start, sub_num));
        value_start = INT_MAX;
      }
    }
    i++;
  }
  return result;
}

}  // namespace base