#ifndef LYNX_BASE_STRING_NUMBER_CONVERT_H_
#define LYNX_BASE_STRING_NUMBER_CONVERT_H_

#include <string>

namespace base {
bool StringToDouble(const std::string& input, double& output);
bool StringToInt(const std::string& input, int64_t& output, uint8_t base);
}  // namespace base
#endif  //  LYNX_BASE_STRING_NUMBER_CONVERT_H_