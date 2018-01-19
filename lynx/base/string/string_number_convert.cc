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

}  // namespace base