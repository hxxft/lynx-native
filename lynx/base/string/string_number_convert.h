#ifndef LYNX_BASE_STRING_NUMBER_CONVERT_H_
#define LYNX_BASE_STRING_NUMBER_CONVERT_H_

#include <string>
#include <vector>

namespace base {
bool StringToDouble(const std::string& input, double& output);
bool StringToInt(const std::string& input, int64_t& output, uint8_t base);
std::vector<std::string> SplitString(const std::string& target, char separator);
}  // namespace base
#endif  //  LYNX_BASE_STRING_NUMBER_CONVERT_H_