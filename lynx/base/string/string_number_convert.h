#ifndef LYNX_BASE_STRING_NUMBER_CONVERT_H_
#define LYNX_BASE_STRING_NUMBER_CONVERT_H_

#include <string>

namespace base {
  bool StringToDouble(const std::string& input, double& output);
}
#endif //  LYNX_BASE_STRING_NUMBER_CONVERT_H_